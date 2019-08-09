#ifndef OPTION_HPP
#define OPTION_HPP

template<typename T>
class Some {
public:
    Some(const Some<T>&) = delete;

    Some(const T&) = delete;

    Some(T&&) : value_(value) {}

private:
    template<typename U>
    friend class Result;

    T value_;
};

class None {
public:
    None() {}
};

template<typename T>
class Option {
public:
    Option(const None&) : is_some_(false)
#ifndef RESULT_OPTION_MULTIPLE_MOVE
    , moved_out_(false)
#endif
    {}

    Option(Some&& value) : value_(value.value_), is_some_(true)
#ifndef RESULT_OPTION_MULTIPLE_MOVE
    , moved_out_(false)
#endif
    {}

    T value() {
        assert(is_some_);
#ifndef RESULT_OPTION_MULTIPLE_MOVE
        assert(!moved_out_);
        moved_out_ = true;
#endif
        return std::move(value_);
    }

    template<typename U>
    Option<U> map(std::function<U(T)> f) {
        if (is_some_) {
#ifndef RESULT_OPTION_MULTIPLE_MOVE
            assert(!moved_out_);
            moved_out_ = true;
#endif
            return Some<U>(std::move(f(std::move(value_))));
        }
        
        return None();
    }

    template<typename U>
    Option<U> and_then(std::function<Option<U>(T)> f) {
        if (is_some_) {
#ifndef RESULT_OPTION_MULTIPLE_MOVE
            assert(!moved_out_);
            moved_out_ = true;
#endif
            return f(std::move(value_));
        }
        
        return None();
    }

    bool is_some() const {
        return is_some_;
    }

private:
    union {
        T value_;
        char dummy_;
    };
    
    bool is_some_;
#ifndef RESULT_OPTION_MULTIPLE_MOVE
    bool moved_out_;
#endif
};

#endif