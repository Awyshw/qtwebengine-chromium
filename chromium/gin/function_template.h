// This file was GENERATED by command:
//     pump.py function_template.h.pump
// DO NOT EDIT BY HAND!!!



#ifndef GIN_FUNCTION_TEMPLATE_H_
#define GIN_FUNCTION_TEMPLATE_H_

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/callback.h"
#include "base/logging.h"
#include "gin/arguments.h"
#include "gin/converter.h"
#include "gin/gin_export.h"
#include "gin/handle.h"
#include "gin/public/gin_embedders.h"
#include "gin/public/wrapper_info.h"
#include "gin/wrappable.h"

#include "v8/include/v8.h"

namespace gin {

class PerIsolateData;

enum CreateFunctionTemplateFlags {
  HolderIsFirstArgument = 1 << 0,
};

namespace internal {

template<typename T>
struct CallbackParamTraits {
  typedef T LocalType;
};
template<typename T>
struct CallbackParamTraits<const T&> {
  typedef T LocalType;
};
template<typename T>
struct CallbackParamTraits<const T*> {
  typedef T* LocalType;
};


// CallbackHolder and CallbackHolderBase are used to pass a base::Callback from
// CreateFunctionTemplate through v8 (via v8::FunctionTemplate) to
// DispatchToCallback, where it is invoked.
//
// v8::FunctionTemplate only supports passing void* as data so how do we know
// when to delete the base::Callback? That's where CallbackHolderBase comes in.
// It inherits from Wrappable, which delete itself when both (a) the refcount
// via base::RefCounted has dropped to zero, and (b) there are no more
// JavaScript references in V8.

// This simple base class is used so that we can share a single object template
// among every CallbackHolder instance.
class GIN_EXPORT CallbackHolderBase : public Wrappable<CallbackHolderBase> {
 public:
  static WrapperInfo kWrapperInfo;
 protected:
  virtual ~CallbackHolderBase() {}
};

template<typename Sig>
class CallbackHolder : public CallbackHolderBase {
 public:
  CallbackHolder(const base::Callback<Sig>& callback, int flags)
      : callback(callback), flags(flags) {}
  base::Callback<Sig> callback;
  int flags;
 private:
  virtual ~CallbackHolder() {}
};


// This set of templates invokes a base::Callback, converts the return type to a
// JavaScript value, and returns that value to script via the provided
// gin::Arguments object.
//
// In C++, you can declare the function foo(void), but you can't pass a void
// expression to foo. As a result, we must specialize the case of Callbacks that
// have the void return type.
template<typename R, typename P1 = void, typename P2 = void,
    typename P3 = void, typename P4 = void>
struct Invoker {
  inline static void Go(
      Arguments* args,
      const base::Callback<R(P1, P2, P3, P4)>& callback,
      const P1& a1,
      const P2& a2,
      const P3& a3,
      const P4& a4) {
    args->Return(callback.Run(a1, a2, a3, a4));
  }
};
template<typename P1, typename P2, typename P3, typename P4>
struct Invoker<void, P1, P2, P3, P4> {
  inline static void Go(
      Arguments* args,
      const base::Callback<void(P1, P2, P3, P4)>& callback,
      const P1& a1,
      const P2& a2,
      const P3& a3,
      const P4& a4) {
    callback.Run(a1, a2, a3, a4);
  }
};

template<typename R, typename P1, typename P2, typename P3>
struct Invoker<R, P1, P2, P3, void> {
  inline static void Go(
      Arguments* args,
      const base::Callback<R(P1, P2, P3)>& callback,
      const P1& a1,
      const P2& a2,
      const P3& a3) {
    args->Return(callback.Run(a1, a2, a3));
  }
};
template<typename P1, typename P2, typename P3>
struct Invoker<void, P1, P2, P3, void> {
  inline static void Go(
      Arguments* args,
      const base::Callback<void(P1, P2, P3)>& callback,
      const P1& a1,
      const P2& a2,
      const P3& a3) {
    callback.Run(a1, a2, a3);
  }
};

template<typename R, typename P1, typename P2>
struct Invoker<R, P1, P2, void, void> {
  inline static void Go(
      Arguments* args,
      const base::Callback<R(P1, P2)>& callback,
      const P1& a1,
      const P2& a2) {
    args->Return(callback.Run(a1, a2));
  }
};
template<typename P1, typename P2>
struct Invoker<void, P1, P2, void, void> {
  inline static void Go(
      Arguments* args,
      const base::Callback<void(P1, P2)>& callback,
      const P1& a1,
      const P2& a2) {
    callback.Run(a1, a2);
  }
};

template<typename R, typename P1>
struct Invoker<R, P1, void, void, void> {
  inline static void Go(
      Arguments* args,
      const base::Callback<R(P1)>& callback,
      const P1& a1) {
    args->Return(callback.Run(a1));
  }
};
template<typename P1>
struct Invoker<void, P1, void, void, void> {
  inline static void Go(
      Arguments* args,
      const base::Callback<void(P1)>& callback,
      const P1& a1) {
    callback.Run(a1);
  }
};

template<typename R>
struct Invoker<R, void, void, void, void> {
  inline static void Go(
      Arguments* args,
      const base::Callback<R()>& callback) {
    args->Return(callback.Run());
  }
};
template<>
struct Invoker<void, void, void, void, void> {
  inline static void Go(
      Arguments* args,
      const base::Callback<void()>& callback) {
    callback.Run();
  }
};


template<typename T>
bool GetNextArgument(Arguments* args, int create_flags, bool is_first,
                     T* result) {
  if (is_first && (create_flags & HolderIsFirstArgument) != 0) {
    return args->GetHolder(result);
  } else {
    return args->GetNext(result);
  }
}

// For advanced use cases, we allow callers to request the unparsed Arguments
// object and poke around in it directly.
inline bool GetNextArgument(Arguments* args, int create_flags, bool is_first,
                            Arguments* result) {
  *result = *args;
  return true;
}


// DispatchToCallback converts all the JavaScript arguments to C++ types and
// invokes the base::Callback.
template<typename Sig>
struct Dispatcher {
};

template<typename R>
struct Dispatcher<R()> {
  static void DispatchToCallback(
      const v8::FunctionCallbackInfo<v8::Value>& info) {
    Arguments args(info);
    CallbackHolderBase* holder_base = NULL;
    CHECK(args.GetData(&holder_base));

    typedef CallbackHolder<R()> HolderT;
    HolderT* holder = static_cast<HolderT*>(holder_base);

    Invoker<R>::Go(&args, holder->callback);
  }
};

template<typename R, typename P1>
struct Dispatcher<R(P1)> {
  static void DispatchToCallback(
      const v8::FunctionCallbackInfo<v8::Value>& info) {
    Arguments args(info);
    CallbackHolderBase* holder_base = NULL;
    CHECK(args.GetData(&holder_base));

    typedef CallbackHolder<R(P1)> HolderT;
    HolderT* holder = static_cast<HolderT*>(holder_base);

    typename CallbackParamTraits<P1>::LocalType a1;
    if (!GetNextArgument(&args, holder->flags, true, &a1)) {
      args.ThrowError();
      return;
    }

    Invoker<R, P1>::Go(&args, holder->callback, a1);
  }
};

template<typename R, typename P1, typename P2>
struct Dispatcher<R(P1, P2)> {
  static void DispatchToCallback(
      const v8::FunctionCallbackInfo<v8::Value>& info) {
    Arguments args(info);
    CallbackHolderBase* holder_base = NULL;
    CHECK(args.GetData(&holder_base));

    typedef CallbackHolder<R(P1, P2)> HolderT;
    HolderT* holder = static_cast<HolderT*>(holder_base);

    typename CallbackParamTraits<P1>::LocalType a1;
    typename CallbackParamTraits<P2>::LocalType a2;
    if (!GetNextArgument(&args, holder->flags, true, &a1) ||
        !GetNextArgument(&args, holder->flags, false, &a2)) {
      args.ThrowError();
      return;
    }

    Invoker<R, P1, P2>::Go(&args, holder->callback, a1, a2);
  }
};

template<typename R, typename P1, typename P2, typename P3>
struct Dispatcher<R(P1, P2, P3)> {
  static void DispatchToCallback(
      const v8::FunctionCallbackInfo<v8::Value>& info) {
    Arguments args(info);
    CallbackHolderBase* holder_base = NULL;
    CHECK(args.GetData(&holder_base));

    typedef CallbackHolder<R(P1, P2, P3)> HolderT;
    HolderT* holder = static_cast<HolderT*>(holder_base);

    typename CallbackParamTraits<P1>::LocalType a1;
    typename CallbackParamTraits<P2>::LocalType a2;
    typename CallbackParamTraits<P3>::LocalType a3;
    if (!GetNextArgument(&args, holder->flags, true, &a1) ||
        !GetNextArgument(&args, holder->flags, false, &a2) ||
        !GetNextArgument(&args, holder->flags, false, &a3)) {
      args.ThrowError();
      return;
    }

    Invoker<R, P1, P2, P3>::Go(&args, holder->callback, a1, a2, a3);
  }
};

template<typename R, typename P1, typename P2, typename P3, typename P4>
struct Dispatcher<R(P1, P2, P3, P4)> {
  static void DispatchToCallback(
      const v8::FunctionCallbackInfo<v8::Value>& info) {
    Arguments args(info);
    CallbackHolderBase* holder_base = NULL;
    CHECK(args.GetData(&holder_base));

    typedef CallbackHolder<R(P1, P2, P3, P4)> HolderT;
    HolderT* holder = static_cast<HolderT*>(holder_base);

    typename CallbackParamTraits<P1>::LocalType a1;
    typename CallbackParamTraits<P2>::LocalType a2;
    typename CallbackParamTraits<P3>::LocalType a3;
    typename CallbackParamTraits<P4>::LocalType a4;
    if (!GetNextArgument(&args, holder->flags, true, &a1) ||
        !GetNextArgument(&args, holder->flags, false, &a2) ||
        !GetNextArgument(&args, holder->flags, false, &a3) ||
        !GetNextArgument(&args, holder->flags, false, &a4)) {
      args.ThrowError();
      return;
    }

    Invoker<R, P1, P2, P3, P4>::Go(&args, holder->callback, a1, a2, a3, a4);
  }
};

}  // namespace internal


// This should be called once per-isolate to initialize the function template
// system.
GIN_EXPORT void InitFunctionTemplates(PerIsolateData* isolate_data);


// CreateFunctionTemplate creates a v8::FunctionTemplate that will create
// JavaScript functions that execute a provided C++ function or base::Callback.
// JavaScript arguments are automatically converted via gin::Converter, as is
// the return value of the C++ function, if any.
template<typename Sig>
v8::Local<v8::FunctionTemplate> CreateFunctionTemplate(
    v8::Isolate* isolate, const base::Callback<Sig> callback,
    int callback_flags = 0) {
  typedef internal::CallbackHolder<Sig> HolderT;
  gin::Handle<HolderT> holder = CreateHandle(
      isolate, new HolderT(callback, callback_flags));
  return v8::FunctionTemplate::New(
      &internal::Dispatcher<Sig>::DispatchToCallback,
      ConvertToV8<internal::CallbackHolderBase*>(isolate, holder.get()));
}

}  // namespace gin

#endif  // GIN_FUNCTION_TEMPLATE_H_