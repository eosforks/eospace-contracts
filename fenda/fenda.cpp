#include <eosiolib/eosio.hpp>
#include <eosiolib/types.hpp>
#include <eosiolib/contract.hpp>
#include <eosiolib/asset.hpp>
#include <vector>
#include <algorithm>
#include <string>

// #define ASSET_TYPE S(4,EOS)

#define STATE_CREATED ("created")
#define STATE_CANCELED ("canceled")
#define STATE_REFUSED ("refused")
#define STATE_ANSWERED ("answered")
#define STATE_ACCEPTED ("accepted")
#define STATE_REJECTED ("rejected")
class fenda : public eosio::contract
{
  public:
    fenda(account_name self) : eosio::contract(self),
                               questions(_self, _self),
                               users(_self, _self){};
    // @abi action
    void regist(account_name contract_name, account_name name, std::string &description)
    {
        eosio::require_auth(name);
        eosio_assert(users.find(name) == users.end(), "already registered.");

        users.emplace(name, [&](auto &u) {
            u.id = name;
            u.description = description;
        });

        eosio::asset bet(1000000,S(4,SEK));
        offerasset(_self, name, contract_name, bet);
    }

    // @abi action
    void ask(account_name contract_name, account_name asker, account_name responder, 
             std::string &title, std::string &brief, uint64_t time, const eosio::asset &bet)
    {
        eosio::require_auth(asker);
        eosio_assert(asker != responder, "asker should not be the same as responder.");
        eosio_assert(bet.symbol == S(4, SEK), "invalid asset type.");
        eosio_assert((bet.amount >= int64_t(0)), "invalide asset amount.");

        auto iter_asker = users.find(asker);
        eosio_assert(iter_asker != users.end(), "asker not found, have you registered?");
        auto iter_responder = users.find(responder);
        eosio_assert(iter_responder != users.end(), "responder not found.");

        // offer asset to contract account
        offerasset(asker, _self, contract_name, bet);

        auto question_id = getNextQuestionID();
        questions.emplace(asker, [&](auto &q) {
            q.id = question_id;
            q.asker = asker;
            q.responder = responder;
            q.title = title;
            q.brief = brief;
            q.ask_time = time;
            q.value = bet;
            q.setCreated();
        });

        users.modify(iter_asker, asker, [&](auto &u) {
            u.questions.push_back(question_id);
        });

        users.modify(iter_responder, asker, [&](auto &u) {
            u.answers.push_back(question_id);
        });
    }
    // @abi action
    void cancel(account_name contract_name, account_name asker, uint64_t question_id)
    {
        eosio::require_auth(asker);

        auto iter = questions.find(question_id);
        eosio_assert(iter != questions.end(), "no such question.");
        eosio_assert(iter->asker == asker, "this question does not belong to you");
        eosio_assert(iter->isCreated(), "question uncancelable.");

        auto iter_users = users.find(iter->responder);
        auto todelete_id = iter->id;
        eosio_assert(iter_users != users.end(), "responder not found.");

        auto bet = iter->value;
        offerasset(_self, asker, contract_name, bet);

        questions.modify(iter, asker, [&](auto &q) {
            q.setCanceled();
            q.clearValue();
        });

        users.modify(iter_users, asker, [&](auto &u) {
            u.answerdrop(todelete_id);
        });
    }

    void refuse(account_name contract_name, account_name responder, uint64_t question_id)
    {
        eosio::require_auth(responder);

        auto iter = questions.find(question_id);
        eosio_assert(iter != questions.end(), "no such question.");
        eosio_assert(iter->responder == responder, "this question does not belong to you");
        eosio_assert(iter->isCreated(), "question unrefuseable.");

        auto bet = iter->value;
        auto asker = iter->asker;
        offerasset(_self, asker, contract_name, bet);

        questions.modify(iter, responder, [&](auto &q) {
            q.setRefused();
            q.clearValue();
        });
    }

    // @abi action
    void answer(account_name responder, uint64_t question_id, std::string &answer, uint64_t time)
    {
        eosio::require_auth(responder);

        auto iter = questions.find(question_id);
        eosio_assert(iter != questions.end(), "no such question.");
        eosio_assert(iter->responder == responder, "this question does not need you to answer");
        eosio_assert(iter->isCreated(), "can not answer this question");

        questions.modify(iter, responder, [&](auto &q) {
            q.setAnswer(answer, time);
        });
    }

    // @abi action
    void comment(account_name contract_name, account_name asker, uint64_t question_id, std::string &comment)
    {
        eosio::require_auth(asker);

        eosio_assert(comment == STATE_ACCEPTED || comment == STATE_REJECTED, "comment type err");

        auto iter = questions.find(question_id);
        eosio_assert(iter != questions.end(), "no such question.");
        eosio_assert(iter->asker == asker, "this question does not need you to comment");
        eosio_assert(iter->isAnswered(), "question have not been answered.");

        auto responder = iter->responder;
        auto bet = iter->value;
        questions.modify(iter, asker, [&](auto &q) {
            q.setComment((question::isAcceptFlag(comment) ? true : false));
            q.clearValue();
        });

        if (question::isAcceptFlag(comment))
        {
            offerasset(_self, responder, contract_name, bet);
        }
        else
        {
            offerasset(_self, asker, contract_name, bet);
        }
    }

    // @abi action
    void like(account_name user, uint64_t question_id)
    {
        eosio::require_auth(user);
        auto iter_user = users.find(user);
        eosio_assert(iter_user != users.end(), "user not found");
        auto iter_question = questions.find(question_id);
        eosio_assert(iter_question != questions.end(), "question not found");
        eosio_assert((iter_question->isAnswered() || iter_question->isCommented()), "question not answered.");

        users.modify(iter_user, user, [&](auto &u) { u.addLike(question_id); });
        questions.modify(iter_question, user, [&](auto &q) { q.addLike(user); });
    }

    // @abi action
    void dislike(account_name user, uint64_t question_id)
    {
        eosio::require_auth(user);
        auto iter_user = users.find(user);
        eosio_assert(iter_user != users.end(), "user not found");
        auto iter_question = questions.find(question_id);
        eosio_assert(iter_question != questions.end(), "question not found");
        eosio_assert((iter_question->isAnswered() || iter_question->isCommented()), "question not answered.");

        users.modify(iter_user, user, [&](auto &u) { u.addDislike(question_id); });
        questions.modify(iter_question, user, [&](auto &q) { q.addDislike(user); });
    }

  private:
    /**
 * state FSM
 * 
 * created --(cancel/refuse/answer)--|--- canceled
 *                                   |--- refused
 *                                   |--- answered --(comment)--||--- accepted
 *                                                              ||--- rejected
 */

    // @abi table question i64
    struct question
    {
        uint64_t id;
        account_name asker;
        account_name responder;
        std::string title;
        std::string brief;
        eosio::asset value;
        std::string answer;
        uint64_t ask_time;
        uint64_t answer_time;
        std::string state;
        uint64_t like;
        uint64_t dislike;
        std::vector<account_name> like_users;
        std::vector<account_name> dislike_users;

        EOSLIB_SERIALIZE(question, (id)(asker)(responder)(title)(brief)(value)(answer)(ask_time)(answer_time)(state)(like)(dislike)(like_users)(dislike_users))

        account_name primary_key() const
        {
            return id;
        }

        void setCreated()
        {
            state = STATE_CREATED;
        }

        void setAnswer(std::string &msg, uint64_t time)
        {
            answer = msg;
            answer_time = time;
            state = STATE_ANSWERED;
        }

        void setComment(bool accept)
        {
            state = (accept ? STATE_ACCEPTED : STATE_REJECTED);
        }

        void setCanceled()
        {
            state = STATE_CANCELED;
        }

        void setRefused()
        {
            state = STATE_REFUSED;
        }

        bool isCreated() const
        {
            return state == STATE_CREATED;
        }

        bool isAnswered() const
        {
            return state == STATE_ANSWERED;
        }

        bool isCommented() const
        {
            return state == STATE_ACCEPTED || state == STATE_REJECTED;
        }

        bool isAccepted() const
        {
            return isAcceptFlag(state);
        }

        static bool isAcceptFlag(const std::string &comment)
        {
            return comment == STATE_ACCEPTED;
        }

        bool isCanceled() const
        {
            return state == STATE_CANCELED;
        }

        bool isRefused() const
        {
            return state == STATE_REFUSED;
        }

        void clearValue()
        {
            value = eosio::asset(0,S(4,SEK));
        }

        void addLike(account_name user)
        {
            if (std::find(like_users.begin(), like_users.end(), user) == like_users.end() && std::find(dislike_users.begin(), dislike_users.end(), user) == dislike_users.end())
            {
                like = like + 1;
                like_users.push_back(user);
            }
        }

        void addDislike(account_name user)
        {
            if (std::find(like_users.begin(), like_users.end(), user) == like_users.end() && std::find(dislike_users.begin(), dislike_users.end(), user) == dislike_users.end())
            {
                dislike = dislike + 1;
                dislike_users.push_back(user);
            }
        }

        // static const std::string STATE_CREATED;
        // static const std::string STATE_CANCELED;
        // static const std::string STATE_REFUSED;
        // static const std::string STATE_ANSWERED;
        // static const std::string STATE_ACCEPTED;
        // static const std::string STATE_REJECTED;
    };

    static constexpr account_name QUESTION_TABLE_NAME = N(question);
    using question_list = eosio::multi_index<QUESTION_TABLE_NAME, question>;

    // @abi table user i64
    struct user
    {
        account_name id;
        std::string description;
        std::vector<uint64_t> questions;
        std::vector<uint64_t> answers;

        uint64_t like;
        uint64_t dislike;
        std::vector<uint64_t> like_questions;
        std::vector<uint64_t> dislike_questions;

        EOSLIB_SERIALIZE(user, (id)(description)(questions)(answers)(like)(dislike)(like_questions)(dislike_questions))

        account_name primary_key() const
        {
            return id;
        }

        void answerdrop(uint64_t id)
        {
            answers.erase(std::find(answers.begin(), answers.end(), id));
        }

        void addLike(uint64_t question_id)
        {
            if (std::find(like_questions.begin(), like_questions.end(), question_id) == like_questions.end() && std::find(dislike_questions.begin(), dislike_questions.end(), question_id) == dislike_questions.end())
            {
                like = like + 1;
                like_questions.push_back(question_id);
            }
        }

        void addDislike(uint64_t question_id)
        {
            if (std::find(like_questions.begin(), like_questions.end(), question_id) == like_questions.end() && std::find(dislike_questions.begin(), dislike_questions.end(), question_id) == dislike_questions.end())
            {
                dislike = dislike + 1;
                dislike_questions.push_back(question_id);
            }
        }
    };
    static constexpr account_name USER_TABLE_NAME = N(user);
    using user_list = eosio::multi_index<USER_TABLE_NAME, user>;

    question_list questions;
    user_list users;

    /////////////////////////////////

    uint64_t getNextQuestionID()
    {
        auto iter_end = questions.end();
        if (iter_end == questions.begin())
        {
            return 1;
        }
        else
        {
            return (--iter_end)->id + 1;
        }
    }

    void offerasset(account_name from, account_name to, account_name contract_name, const eosio::asset &bet) const
    {
        eosio::action(
            eosio::permission_level{from, N(active)},
            contract_name, N(transfer),
            std::make_tuple(from, to, bet, std::string("offer bet")))
            .send();
    }
};

// const std::string fenda::question::STATE_CREATED = "created";
// const std::string fenda::question::STATE_CANCELED = "canceled";
// const std::string fenda::question::STATE_REFUSED = "refused";
// const std::string fenda::question::STATE_ANSWERED = "answered";
// const std::string fenda::question::STATE_ACCEPTED = "accepted";
// const std::string fenda::question::STATE_REJECTED = "rejected";

EOSIO_ABI(fenda, (regist)(ask)(cancel)(refuse)(answer)(comment)(like)(dislike))