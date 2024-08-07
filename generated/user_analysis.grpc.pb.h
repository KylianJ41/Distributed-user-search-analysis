// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: user_analysis.proto
#ifndef GRPC_user_5fanalysis_2eproto__INCLUDED
#define GRPC_user_5fanalysis_2eproto__INCLUDED

#include "user_analysis.pb.h"

#include <functional>
#include <grpcpp/generic/async_generic_service.h>
#include <grpcpp/support/async_stream.h>
#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/support/client_callback.h>
#include <grpcpp/client_context.h>
#include <grpcpp/completion_queue.h>
#include <grpcpp/support/message_allocator.h>
#include <grpcpp/support/method_handler.h>
#include <grpcpp/impl/proto_utils.h>
#include <grpcpp/impl/rpc_method.h>
#include <grpcpp/support/server_callback.h>
#include <grpcpp/impl/server_callback_handlers.h>
#include <grpcpp/server_context.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/status.h>
#include <grpcpp/support/stub_options.h>
#include <grpcpp/support/sync_stream.h>

namespace user_analysis {

class UserAnalysisService final {
 public:
  static constexpr char const* service_full_name() {
    return "user_analysis.UserAnalysisService";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status GetTask(::grpc::ClientContext* context, const ::user_analysis::GetTaskRequest& request, ::user_analysis::Task* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::user_analysis::Task>> AsyncGetTask(::grpc::ClientContext* context, const ::user_analysis::GetTaskRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::user_analysis::Task>>(AsyncGetTaskRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::user_analysis::Task>> PrepareAsyncGetTask(::grpc::ClientContext* context, const ::user_analysis::GetTaskRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::user_analysis::Task>>(PrepareAsyncGetTaskRaw(context, request, cq));
    }
    virtual ::grpc::Status SubmitResult(::grpc::ClientContext* context, const ::user_analysis::TaskResult& request, ::user_analysis::SubmitResultResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::user_analysis::SubmitResultResponse>> AsyncSubmitResult(::grpc::ClientContext* context, const ::user_analysis::TaskResult& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::user_analysis::SubmitResultResponse>>(AsyncSubmitResultRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::user_analysis::SubmitResultResponse>> PrepareAsyncSubmitResult(::grpc::ClientContext* context, const ::user_analysis::TaskResult& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::user_analysis::SubmitResultResponse>>(PrepareAsyncSubmitResultRaw(context, request, cq));
    }
    class async_interface {
     public:
      virtual ~async_interface() {}
      virtual void GetTask(::grpc::ClientContext* context, const ::user_analysis::GetTaskRequest* request, ::user_analysis::Task* response, std::function<void(::grpc::Status)>) = 0;
      virtual void GetTask(::grpc::ClientContext* context, const ::user_analysis::GetTaskRequest* request, ::user_analysis::Task* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      virtual void SubmitResult(::grpc::ClientContext* context, const ::user_analysis::TaskResult* request, ::user_analysis::SubmitResultResponse* response, std::function<void(::grpc::Status)>) = 0;
      virtual void SubmitResult(::grpc::ClientContext* context, const ::user_analysis::TaskResult* request, ::user_analysis::SubmitResultResponse* response, ::grpc::ClientUnaryReactor* reactor) = 0;
    };
    typedef class async_interface experimental_async_interface;
    virtual class async_interface* async() { return nullptr; }
    class async_interface* experimental_async() { return async(); }
   private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::user_analysis::Task>* AsyncGetTaskRaw(::grpc::ClientContext* context, const ::user_analysis::GetTaskRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::user_analysis::Task>* PrepareAsyncGetTaskRaw(::grpc::ClientContext* context, const ::user_analysis::GetTaskRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::user_analysis::SubmitResultResponse>* AsyncSubmitResultRaw(::grpc::ClientContext* context, const ::user_analysis::TaskResult& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::user_analysis::SubmitResultResponse>* PrepareAsyncSubmitResultRaw(::grpc::ClientContext* context, const ::user_analysis::TaskResult& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());
    ::grpc::Status GetTask(::grpc::ClientContext* context, const ::user_analysis::GetTaskRequest& request, ::user_analysis::Task* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::user_analysis::Task>> AsyncGetTask(::grpc::ClientContext* context, const ::user_analysis::GetTaskRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::user_analysis::Task>>(AsyncGetTaskRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::user_analysis::Task>> PrepareAsyncGetTask(::grpc::ClientContext* context, const ::user_analysis::GetTaskRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::user_analysis::Task>>(PrepareAsyncGetTaskRaw(context, request, cq));
    }
    ::grpc::Status SubmitResult(::grpc::ClientContext* context, const ::user_analysis::TaskResult& request, ::user_analysis::SubmitResultResponse* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::user_analysis::SubmitResultResponse>> AsyncSubmitResult(::grpc::ClientContext* context, const ::user_analysis::TaskResult& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::user_analysis::SubmitResultResponse>>(AsyncSubmitResultRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::user_analysis::SubmitResultResponse>> PrepareAsyncSubmitResult(::grpc::ClientContext* context, const ::user_analysis::TaskResult& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::user_analysis::SubmitResultResponse>>(PrepareAsyncSubmitResultRaw(context, request, cq));
    }
    class async final :
      public StubInterface::async_interface {
     public:
      void GetTask(::grpc::ClientContext* context, const ::user_analysis::GetTaskRequest* request, ::user_analysis::Task* response, std::function<void(::grpc::Status)>) override;
      void GetTask(::grpc::ClientContext* context, const ::user_analysis::GetTaskRequest* request, ::user_analysis::Task* response, ::grpc::ClientUnaryReactor* reactor) override;
      void SubmitResult(::grpc::ClientContext* context, const ::user_analysis::TaskResult* request, ::user_analysis::SubmitResultResponse* response, std::function<void(::grpc::Status)>) override;
      void SubmitResult(::grpc::ClientContext* context, const ::user_analysis::TaskResult* request, ::user_analysis::SubmitResultResponse* response, ::grpc::ClientUnaryReactor* reactor) override;
     private:
      friend class Stub;
      explicit async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class async* async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class async async_stub_{this};
    ::grpc::ClientAsyncResponseReader< ::user_analysis::Task>* AsyncGetTaskRaw(::grpc::ClientContext* context, const ::user_analysis::GetTaskRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::user_analysis::Task>* PrepareAsyncGetTaskRaw(::grpc::ClientContext* context, const ::user_analysis::GetTaskRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::user_analysis::SubmitResultResponse>* AsyncSubmitResultRaw(::grpc::ClientContext* context, const ::user_analysis::TaskResult& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::user_analysis::SubmitResultResponse>* PrepareAsyncSubmitResultRaw(::grpc::ClientContext* context, const ::user_analysis::TaskResult& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_GetTask_;
    const ::grpc::internal::RpcMethod rpcmethod_SubmitResult_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status GetTask(::grpc::ServerContext* context, const ::user_analysis::GetTaskRequest* request, ::user_analysis::Task* response);
    virtual ::grpc::Status SubmitResult(::grpc::ServerContext* context, const ::user_analysis::TaskResult* request, ::user_analysis::SubmitResultResponse* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_GetTask : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_GetTask() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_GetTask() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetTask(::grpc::ServerContext* /*context*/, const ::user_analysis::GetTaskRequest* /*request*/, ::user_analysis::Task* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestGetTask(::grpc::ServerContext* context, ::user_analysis::GetTaskRequest* request, ::grpc::ServerAsyncResponseWriter< ::user_analysis::Task>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_SubmitResult : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_SubmitResult() {
      ::grpc::Service::MarkMethodAsync(1);
    }
    ~WithAsyncMethod_SubmitResult() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status SubmitResult(::grpc::ServerContext* /*context*/, const ::user_analysis::TaskResult* /*request*/, ::user_analysis::SubmitResultResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestSubmitResult(::grpc::ServerContext* context, ::user_analysis::TaskResult* request, ::grpc::ServerAsyncResponseWriter< ::user_analysis::SubmitResultResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_GetTask<WithAsyncMethod_SubmitResult<Service > > AsyncService;
  template <class BaseClass>
  class WithCallbackMethod_GetTask : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_GetTask() {
      ::grpc::Service::MarkMethodCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::user_analysis::GetTaskRequest, ::user_analysis::Task>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::user_analysis::GetTaskRequest* request, ::user_analysis::Task* response) { return this->GetTask(context, request, response); }));}
    void SetMessageAllocatorFor_GetTask(
        ::grpc::MessageAllocator< ::user_analysis::GetTaskRequest, ::user_analysis::Task>* allocator) {
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(0);
      static_cast<::grpc::internal::CallbackUnaryHandler< ::user_analysis::GetTaskRequest, ::user_analysis::Task>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~WithCallbackMethod_GetTask() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetTask(::grpc::ServerContext* /*context*/, const ::user_analysis::GetTaskRequest* /*request*/, ::user_analysis::Task* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* GetTask(
      ::grpc::CallbackServerContext* /*context*/, const ::user_analysis::GetTaskRequest* /*request*/, ::user_analysis::Task* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithCallbackMethod_SubmitResult : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_SubmitResult() {
      ::grpc::Service::MarkMethodCallback(1,
          new ::grpc::internal::CallbackUnaryHandler< ::user_analysis::TaskResult, ::user_analysis::SubmitResultResponse>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::user_analysis::TaskResult* request, ::user_analysis::SubmitResultResponse* response) { return this->SubmitResult(context, request, response); }));}
    void SetMessageAllocatorFor_SubmitResult(
        ::grpc::MessageAllocator< ::user_analysis::TaskResult, ::user_analysis::SubmitResultResponse>* allocator) {
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(1);
      static_cast<::grpc::internal::CallbackUnaryHandler< ::user_analysis::TaskResult, ::user_analysis::SubmitResultResponse>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~WithCallbackMethod_SubmitResult() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status SubmitResult(::grpc::ServerContext* /*context*/, const ::user_analysis::TaskResult* /*request*/, ::user_analysis::SubmitResultResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* SubmitResult(
      ::grpc::CallbackServerContext* /*context*/, const ::user_analysis::TaskResult* /*request*/, ::user_analysis::SubmitResultResponse* /*response*/)  { return nullptr; }
  };
  typedef WithCallbackMethod_GetTask<WithCallbackMethod_SubmitResult<Service > > CallbackService;
  typedef CallbackService ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_GetTask : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_GetTask() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_GetTask() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetTask(::grpc::ServerContext* /*context*/, const ::user_analysis::GetTaskRequest* /*request*/, ::user_analysis::Task* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_SubmitResult : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_SubmitResult() {
      ::grpc::Service::MarkMethodGeneric(1);
    }
    ~WithGenericMethod_SubmitResult() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status SubmitResult(::grpc::ServerContext* /*context*/, const ::user_analysis::TaskResult* /*request*/, ::user_analysis::SubmitResultResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_GetTask : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_GetTask() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_GetTask() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetTask(::grpc::ServerContext* /*context*/, const ::user_analysis::GetTaskRequest* /*request*/, ::user_analysis::Task* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestGetTask(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawMethod_SubmitResult : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_SubmitResult() {
      ::grpc::Service::MarkMethodRaw(1);
    }
    ~WithRawMethod_SubmitResult() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status SubmitResult(::grpc::ServerContext* /*context*/, const ::user_analysis::TaskResult* /*request*/, ::user_analysis::SubmitResultResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestSubmitResult(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_GetTask : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_GetTask() {
      ::grpc::Service::MarkMethodRawCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->GetTask(context, request, response); }));
    }
    ~WithRawCallbackMethod_GetTask() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetTask(::grpc::ServerContext* /*context*/, const ::user_analysis::GetTaskRequest* /*request*/, ::user_analysis::Task* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* GetTask(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_SubmitResult : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_SubmitResult() {
      ::grpc::Service::MarkMethodRawCallback(1,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->SubmitResult(context, request, response); }));
    }
    ~WithRawCallbackMethod_SubmitResult() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status SubmitResult(::grpc::ServerContext* /*context*/, const ::user_analysis::TaskResult* /*request*/, ::user_analysis::SubmitResultResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* SubmitResult(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_GetTask : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_GetTask() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler<
          ::user_analysis::GetTaskRequest, ::user_analysis::Task>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::user_analysis::GetTaskRequest, ::user_analysis::Task>* streamer) {
                       return this->StreamedGetTask(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_GetTask() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status GetTask(::grpc::ServerContext* /*context*/, const ::user_analysis::GetTaskRequest* /*request*/, ::user_analysis::Task* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedGetTask(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::user_analysis::GetTaskRequest,::user_analysis::Task>* server_unary_streamer) = 0;
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_SubmitResult : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_SubmitResult() {
      ::grpc::Service::MarkMethodStreamed(1,
        new ::grpc::internal::StreamedUnaryHandler<
          ::user_analysis::TaskResult, ::user_analysis::SubmitResultResponse>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::user_analysis::TaskResult, ::user_analysis::SubmitResultResponse>* streamer) {
                       return this->StreamedSubmitResult(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_SubmitResult() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status SubmitResult(::grpc::ServerContext* /*context*/, const ::user_analysis::TaskResult* /*request*/, ::user_analysis::SubmitResultResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedSubmitResult(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::user_analysis::TaskResult,::user_analysis::SubmitResultResponse>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_GetTask<WithStreamedUnaryMethod_SubmitResult<Service > > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_GetTask<WithStreamedUnaryMethod_SubmitResult<Service > > StreamedService;
};

}  // namespace user_analysis


#endif  // GRPC_user_5fanalysis_2eproto__INCLUDED
