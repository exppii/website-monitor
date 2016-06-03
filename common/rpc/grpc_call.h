//
// Created by 刘优 on 16/5/10.
//

#ifndef WEBSITEMONITOR_COMMON_RPC_GRPC_CALL_H_
#define WEBSITEMONITOR_COMMON_RPC_GRPC_CALL_H_

#include <mutex>

#include <grpc++/grpc++.h>
#include <grpc++/server_builder.h>

#include "common/retcount.h"

namespace webmonitor {

// Represents a pending request with unknown message types.
template <class Service>
class UntypedCall : public RefCounted {
public:
  virtual ~UntypedCall() {}

  // The implementation of this method should use `service` to handle
  // an incoming request, and (perhaps asynchronously) send the
  // response.
  //
  // One reference on `this` is transferred to the callee, and the
  // callee is responsible for releasing it (typically via
  // `Call::SendResponse()`).
  //
  // `ok` is true if the request was received in a "regular event",
  // otherwise false.
  virtual void RequestReceived(Service* service, bool ok) = 0;

  // This method will be called when the response has been sent by
  // `service` and the call is no longer used.
  //
  // `ok` is true if the response sending completed as a "regular
  // event", otherwise it is false.
  void ResponseSent(Service* service, bool ok) {}

  // This method will be called either (i) when the server is notified
  // that the request has been cancelled, or (ii) when the request completes
  // normally. The implementation should distinguish these cases by querying
  // the `grpc::ServerContext` associated with the request.
  virtual void RequestCancelled(Service* service, bool ok) = 0;

  // Associates a tag in a `::grpc::CompletionQueue` with a callback
  // for an incoming RPC.  A Tag owns a reference on the corresponding
  // Call object.
  class Tag {
  public:
    using Callback = void (UntypedCall::*)(Service*, bool);

    // Creates a new `Tag` for the given `UntypedCall`. When the
    // request associated with this tag is complete, `callback` will
    // be called.
    Tag(UntypedCall* call, Callback callback)
        : call_(call), callback_(callback) {
      call_->ref();
    }

    ~Tag() { call_->unref(); }

    // Calls the callback associated with this tag.
    //
    // The callback takes ownership of `this->call_`.
    void OnCompleted(Service* service, bool ok) {
      (call_->*callback_)(service, ok);
    }

  private:
    UntypedCall* const call_;  // `this` owns one reference.
    Callback callback_;
  };
};

// Represents a pending call with known request and response message
// types, and a known request-handling method.
template <class Service, class GrpcService, class RequestMessage,
    class ResponseMessage>
class Call : public UntypedCall<Service> {
public:
  // Represents the generic signature of a generated
  // `GrpcService::RequestFoo()` method, where `Foo` is the name of an
  // RPC method.
  using EnqueueFunction = void (GrpcService::*)(
      ::grpc::ServerContext*, RequestMessage*,
      ::grpc::ServerAsyncResponseWriter<ResponseMessage>*,
      ::grpc::CompletionQueue*, ::grpc::ServerCompletionQueue*, void*);

  // Represents the generic signature of a `Service::HandleFoo()`
  // method, where `Foo` is the name of an RPC method.
  using HandleRequestFunction = void (Service::*)(
      Call<Service, GrpcService, RequestMessage, ResponseMessage>*);

  Call(HandleRequestFunction handle_request_function)
      : handle_request_function_(handle_request_function), responder_(&ctx_) {}

  virtual ~Call() {}

  void RequestReceived(Service* service, bool ok) override {
    if (ok) {
      this->ref();
      (service->*handle_request_function_)(this);
    }
  }

  void SendResponse(::grpc::Status status) {
    responder_.Finish(response, status,
                      new typename UntypedCall<Service>::Tag(
                          this, &UntypedCall<Service>::ResponseSent));
    this->unref();
  }

  void RequestCancelled(Service* service, bool ok) override {
    if (ctx_.IsCancelled()) {
      std::unique_lock<std::mutex> l(_mtx);
      if (cancel_callback_) {
        cancel_callback_();
      }
    }
    // NOTE(mrry): This can be called before or after RequestReceived, so we
    // release `cancel_tag_` (in order to allow the event loop to free it).
    cancel_tag_.release();
  }

  // Registers `callback` as the function that should be called if and when this
  // call is cancelled by the client.
  void SetCancelCallback(std::function<void()> callback) {
    std::unique_lock<std::mutex> l(_mtx);
    cancel_callback_ = callback;
  }

  // Clears any cancellation callback that has been registered for this call.
  void ClearCancelCallback() {
    std::unique_lock<std::mutex> l(_mtx);
    cancel_callback_ = nullptr;
  }

  // Enqueues a new request for the given service on the given
  // completion queue, using the given `enqueue_function`.
  //
  // The request will be handled with the given
  // `handle_request_function`.
  static void EnqueueRequest(GrpcService* grpc_service,
                             ::grpc::ServerCompletionQueue* cq,
                             EnqueueFunction enqueue_function,
                             HandleRequestFunction handle_request_function,
                             bool supports_cancel) {
    auto call = new Call<Service, GrpcService, RequestMessage, ResponseMessage>(
        handle_request_function);
    if (supports_cancel) {
      call->RegisterCancellationHandler();
    }

    (grpc_service->*enqueue_function)(
        &call->ctx_, &call->request, &call->responder_, cq, cq,
        new typename UntypedCall<Service>::Tag(
            call, &UntypedCall<Service>::RequestReceived));
    call->unref();
  }

  RequestMessage request;
  ResponseMessage response;

private:
  // Creates a completion queue tag for handling cancellation by the client.
  // NOTE: This method must be called before this call is enqueued on a
  // completion queue.
  void RegisterCancellationHandler() {
    cancel_tag_.reset(new typename UntypedCall<Service>::Tag(
        this, &UntypedCall<Service>::RequestCancelled));
    ctx_.AsyncNotifyWhenDone(cancel_tag_.get());
  }

  HandleRequestFunction handle_request_function_;
  ::grpc::ServerContext ctx_;
  ::grpc::ServerAsyncResponseWriter<ResponseMessage> responder_;
  std::mutex _mtx;
  std::function<void()> cancel_callback_ ;//GUARDED_BY(mu_);

  // This tag is initially owned by `*this` and borrowed by
  // `ctx_->AsyncNotifyWhenDone()`. Ownership is transferred to the
  // appropriate service's completion queue after
  // `this->RequestReceived(..., true)` is called.
  std::unique_ptr<typename UntypedCall<Service>::Tag> cancel_tag_;
};


} //namespace


#endif //WEBSITEMONITOR_COMMON_RPC_GRPC_CALL_H_
