#ifndef RESULT_HPP
#define RESULT_HPP

#include <utility>
#include <string>
#include <cassert>
#include <functional>

template<typename T>
class Ok {
public:
    Ok(const Ok<T>&) = delete;

    Ok(const T&) = delete;

    inline Ok(T&& value) : value_(value) {}

private:
    template<typename U>
    friend class Result;

    T value_;
};

class Err {
public:
    Err(const Err&) = delete;

    Err(const std::string&) = delete;

    inline Err(std::string&& value) : value_(value) {}

private:
    template<typename T>
    friend class Result;
    
    std::string value_;
};

template<typename T>
class Result {
public:
    Result(const Result<T>&) = delete;

    Result(Result<T>&& other) 
#ifndef RESULT_MULTIPLE_MOVE
    : moved_out_(false) 
#endif 
    {
#ifndef RESULT_MULTIPLE_MOVE
        assert(!other.moved_out_);
#endif
        if (other.is_ok_) {
            ok_ = std::move(other.ok_);
            is_ok_ = true;
        } else {
            err_ = std::move(other.err_);
            is_ok_ = false;
        }
    }

    Result(Ok<T>&& value) : ok_(std::move(value.value_)), is_ok_(true)
#ifndef RESULT_MULTIPLE_MOVE
    , moved_out_(false)
#endif 
    {}

    Result(Err&& value) : err_(std::move(value.value_)), is_ok_(false)
#ifndef RESULT_MULTIPLE_MOVE
    , moved_out_(false)
#endif 
    {}

    ~Result() {
        if (is_ok_) 
            ok_.~T();
        else
            err_.~basic_string();
    }

    T ok() {
        assert(is_ok_);
#ifndef RESULT_MULTIPLE_MOVE
        assert(!moved_out_);
        moved_out_ = true;
#endif
        
        return std::move(ok_);
    }

    std::string err() {
        assert(!is_ok_);
#ifndef RESULT_MULTIPLE_MOVE
        assert(!moved_out_);
        moved_out_ = true;
#endif

        return std::move(err_);
    }

    template<typename U>
    Result<U> map(std::function<U(T)> f) {
#ifndef RESULT_MULTIPLE_MOVE
        assert(!moved_out_);
        moved_out_ = true;
#endif

        if (is_ok_)
            return Ok<U>(std::move(f(std::move(ok_))));
        else
            return Err(std::move(err_));
    }

    template<typename U>
    Result<U> and_then(std::function<Result<U>(T)> f) {
#ifndef RESULT_MULTIPLE_MOVE
        assert(!moved_out_);
        moved_out_ = true;
#endif

        if (is_ok_)
            return f(std::move(ok_));
        else
            return Err(std::move(err_));
    }

    bool is_ok() const {
        return is_ok_;
    }

    bool is_err() const {
        return !is_ok_;
    }

    operator bool() {
        return is_ok_;
    }

    T operator*() {
        return std::move(ok());
    }

private:
    union {
        T ok_;
        std::string err_;
    };

    bool is_ok_;
#ifndef RESULT_MULTIPLE_MOVE
    bool moved_out_;
#endif
};

#endif