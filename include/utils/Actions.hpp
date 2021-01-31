#ifndef TNT_ACTIONS_HPP
#define TNT_ACTIONS_HPP

#include <deque>

// TODO: Make resumable Actions using C++20 coroutines.
// TODO: Separate ResumableAction from Action.
// TODO: Separate actions in lanes.
// TODO: blocks/blocked_by

// IDEA:
// Trigger a new action when an action is finished(that's why End() is for).

namespace tnt
{
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    namespace detail
    {
        class base_action
        {
        public:
            base_action(long long duration_, bool blocks)
                : finished{false}, blocking{blocks}, elapsed{0}, duration{duration_} {}

            virtual ~base_action() noexcept = default;

            virtual void Update(long long elapsed_) = 0;

            inline bool isBlocking() const noexcept { return blocking; }
            inline bool isFinished() const noexcept { return finished; }

        protected:
            inline void Tick(long long elapsed_) noexcept
            {
                if (!finished)
                {
                    elapsed = elapsed + elapsed_;
                    if (elapsed > duration)
                        finished = true;
                }
            }

            bool finished;
            bool blocking;
            unsigned lane;
            long long elapsed;
            long long duration;
        };
    } // namespace detail
#endif

    /// @brief A basic non-blocking action.
    class Action : public detail::base_action
    {
    public:
        /// @brief Create a non-blocking action that lasts @c duration_ milliseconds.
        /// @param duration_ The duration of the action.
        explicit Action(long long duration_) : detail::base_action{duration_, false} {}
    };

    /// @brief An action that blocks other actions from running before it is finished.
    class BlockingAction : public detail::base_action
    {
    public:
        /// @brief Create a non-blocking action that lasts @c duration_ milliseconds.
        /// @param duration_ The duration of the action.
        explicit BlockingAction(long long duration_) : detail::base_action{duration_, true} {}
    };

    /// @brief An action that updates only once and then finishes.
    class SingleRunAction : public Action
    {
    public:
        /// @brief Create an action that runs only once.
        SingleRunAction() : Action{1} {}
    };

    /// @brief A class holding several actions and updating them in order every frame.
    class ActionList final
    {
    public:
        /// @brief Add a non-blocking action to the "list".
        /// @param action The action to add.
        inline void add(Action *action)
        {
            actions.push_back(static_cast<detail::base_action *>(action));
        }

        /// @brief Add a blocking action to the "list".
        /// @param action The action to add.
        inline void add(BlockingAction *action)
        {
            actions.push_back(static_cast<detail::base_action *>(action));
        }

        /// @brief Check if the actionlist is empty.
        /// @return bool
        inline bool empty() const noexcept { return actions.empty(); }

        /// @brief Update all actions to the first blocking action found.
        /// @param time_ The elapsed time since the last Update() call.
        void Update(float time_)
        {
            if (!actions.empty())
                for (auto it{actions.begin()}; it != actions.end(); ++it)
                {
                    if ((*it)->isFinished())
                        it = actions.erase(it);
                    else
                        (*it)->Update(time_);

                    if (!(*it)->isFinished())
                        if ((*it)->isBlocking())
                            return;
                }
        }

    private:
        std::deque<detail::base_action *> actions;
    };

    /// @brief An action that delays the execution of the other actions for a specified time.
    class Delay : public BlockingAction
    {
    public:
        /// @brief Create a delay action.
        /// @param duration_ The delay time in ms.
        explicit Delay(long long duration_) : BlockingAction{duration_} {}

        /// @brief Update the elapsed time of the Delay action.
        /// @param elapsed_ The elapsed time since the last Update() call.
        inline void Update(long long elapsed_) override
        {
            Tick(elapsed_);
        }
    };
} // namespace tnt

#endif //!TNT_ACTIONS_HPP