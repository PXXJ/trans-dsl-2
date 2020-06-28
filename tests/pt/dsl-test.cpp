//
// Created by Darwin Yuan on 2020/6/17.
//

#include <nanobench.h>
//#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch.hpp>
#include "../ut/StupidTransactionContext.h"
#include "../ut/SimpleActionsDefs.h"
#include <trans-dsl/trans-dsl.h>
#include <iostream>


#define SIMPLE_EVENT(n) \
EV_NS::SimpleEventInfo const se_## n{n}

SIMPLE_EVENT(1);
SIMPLE_EVENT(2);
SIMPLE_EVENT(3);
SIMPLE_EVENT(4);
SIMPLE_EVENT(5);
SIMPLE_EVENT(6);
SIMPLE_EVENT(7);
SIMPLE_EVENT(8);
SIMPLE_EVENT(9);

namespace {
   using namespace TSL_NS;

   TEST_CASE("TestSequential") {
      StupidTransactionContext context;

      using Proc = __procedure
      ( __sequential
          ( __wait(1)
             , __wait(2)
             , __wait(3)
             , __wait(4)
             , __wait(5)
             , __wait(6)),
        __finally(__sequential(__wait(7), __wait(8), __wait(9)))
      );

      auto runSequential = [&] {
         Proc proc;
         proc.exec(context);
         proc.handleEvent(context, Event{se_1});
         proc.handleEvent(context, Event{se_2});
         proc.handleEvent(context, Event{se_3});
         proc.handleEvent(context, Event{se_4});
         proc.handleEvent(context, Event{se_5});
         proc.handleEvent(context, Event{se_6});
         proc.handleEvent(context, Event{se_7});
         proc.handleEvent(context, Event{se_8});
         proc.handleEvent(context, Event{se_9});
      };

      SECTION("test") {
         ankerl::nanobench::Bench().epochs(1000).run("run-procedure", [&] {
            runSequential();
         });
      }
   };

   TEST_CASE("TestConcurrent") {
      StupidTransactionContext context;

      using ProcedureAction1 =
      __procedure(
         __wait(1),
         __finally(__asyn(AsyncAction2)));

      using ProcedureAction2 =
      __procedure(
         __wait(2),
         __finally(__asyn(AsyncAction1)));

      using Concurrent = __concurrent(ProcedureAction1, ProcedureAction2);

      const Msg1 msg1{ 10, 20 };
      const EV_NS::ConsecutiveEventInfo eventInfo1{EV_MSG_1, msg1};

      const Msg2 msg2{ 30 };
      const EV_NS::ConsecutiveEventInfo eventInfo2{EV_MSG_2, msg2};

      auto runConcurrent = [&] {
         Concurrent action;

         assert(Result::CONTINUE == action.exec(context));
         assert(Result::CONTINUE == action.handleEvent(context, Event{se_1}));
         assert(Result::CONTINUE == action.handleEvent(context, Event{se_2}));
         assert(Result::CONTINUE == action.handleEvent(context, Event{eventInfo1}));
         assert(Result::SUCCESS == action.handleEvent(context, Event{eventInfo2}));
      };

      SECTION("performance") {
         ankerl::nanobench::Bench().epochs(1000).run("run-concurrent", [&] {
            runConcurrent();
         });
      }
   };
}
