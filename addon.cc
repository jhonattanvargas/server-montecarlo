#include <node.h>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <thread>
#include <ctime>
#include <string>

#define BILLION  1E9;

using namespace std;

using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Value;

//int var = 0;

unsigned long long incircle;
unsigned long long points_per_thread;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *runner(void *threadid) {
  unsigned long long incircle_thread = 0;
  unsigned int rand_state = rand();
  unsigned long long i;
  for (i = 0; i < points_per_thread; i++) {
    double x = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
    double y = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
    if (x * x + y * y < 1) {
      incircle_thread++;
    }
  }
  pthread_mutex_lock(&mutex);
  incircle += incircle_thread;
  pthread_mutex_unlock(&mutex);
  pthread_exit(NULL);
}

void parallel (const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  //var ++;
  //printf("var: %d\n",var);
  //define numero de cpus
  unsigned cpus = std::thread::hardware_concurrency();
  //revisar que los argumentos sean correctos
  if (args.Length() < 1) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  if (!args[0]->IsNumber()) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }
  //fin revisión

  int rc;
  int i;
  unsigned long long totalpoints =  args[0]->NumberValue();
  //printf("cicles : %llu\n", totalpoints);
  points_per_thread = totalpoints / cpus;

  pthread_t threads[cpus];

  struct timespec requestStart, requestEnd;
  //inicio tiempo de medida
  clock_gettime(CLOCK_REALTIME, &requestStart);

  for( i=0; i < cpus; i++ ){
    rc = pthread_create(&threads[i], NULL, runner, (void *) i);
    if (rc){
       cout << "Error:unable to create thread," << rc << endl;
       exit(-1);
    }
  }

  for( i=0; i < cpus; i++ ){
    pthread_join(threads[i],NULL);
  }

  //pthread_mutex_destroy(&mutex);

  double pi = (4. * (double)incircle) / ((double)points_per_thread * cpus);
  //fin tiempo de medida
  clock_gettime(CLOCK_REALTIME, &requestEnd);

  double accum = ( requestEnd.tv_sec - requestStart.tv_sec )
      + ( requestEnd.tv_nsec - requestStart.tv_nsec )
      / BILLION;
  //printf("Pi: %1.10lf\n", pi);
  //printf( "Parallel Time taken: %lf\n", accum );

  free(threads); 
  incircle = 0;
  points_per_thread = 0;

  Local<Object> obj = Object::New(isolate);
  obj->Set(String::NewFromUtf8(isolate, "pi"), Number::New(isolate, pi));
  obj->Set(String::NewFromUtf8(isolate, "time"), Number::New(isolate, accum));
  obj->Set(String::NewFromUtf8(isolate, "cpus"), Number::New(isolate, cpus));

  args.GetReturnValue().Set(obj);
  //pthread_exit(NULL);
}

void serial (const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  //revisar que los argumentos sean correctos
  if (args.Length() < 1) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }
  if (!args[0]->IsNumber()) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }
  //fin revisión

  unsigned long long serial_incircle = 0;
  unsigned long long totalpoints = args[0]->NumberValue();
  unsigned long long i;
  //printf("cicles : %llu\n", totalpoints);
  srand((unsigned)time(NULL));
  unsigned int rand_state = rand();
  //inicio de tiempo de ejecución
  struct timespec requestStart, requestEnd;
  clock_gettime(CLOCK_REALTIME, &requestStart);

  for (i = 0; i < totalpoints; i++) {
    double x = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
    double y = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
    if (x * x + y * y < 1) {
      serial_incircle++;
    }
  }

  double pi = (4. * (double)serial_incircle / (double)totalpoints);

  //fin del tiempo de ejecución
  clock_gettime(CLOCK_REALTIME, &requestEnd);

  double accum = ( requestEnd.tv_sec - requestStart.tv_sec )
    + ( requestEnd.tv_nsec - requestStart.tv_nsec )
    / BILLION;
  //printf("Pi: %1.10lf\n", pi);
  //printf( "Serial Time taken: %lf\n", accum );

  Local<Object> obj = Object::New(isolate);
  obj->Set(String::NewFromUtf8(isolate, "pi"), Number::New(isolate, pi));
  obj->Set(String::NewFromUtf8(isolate, "time"), Number::New(isolate, accum));

  args.GetReturnValue().Set(obj);
}

void Init(Local<Object> exports) {
NODE_SET_METHOD(exports, "parallel", parallel);
NODE_SET_METHOD(exports, "serial", serial);
}

NODE_MODULE(addon, Init)
