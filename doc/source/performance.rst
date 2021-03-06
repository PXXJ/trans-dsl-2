.. _performance:

性能对比
==============

.. code-block:: c++

   __procedure
   ( __sequential
     ( __wait(1)
     , __wait(2)
     , __wait(3)
     , __wait(4)
     , __wait(5)
     , __wait(6))
     , __wait(7))
     , __finally
       ( __sequential
         ( __wait(7)
         , __wait(8)
         , __wait(9)
         , __wait(1)))
   ) a;

1.x:
  ``1225 ns``

2.0:
  ``116 ns``

.. code-block:: c++

   using ProcedureAction1 =
     __procedure(
       __wait(1),
       __finally(__asyn(AsyncAction2)));

   using ProcedureAction2 =
     __procedure(
       __wait(2),
       __finally(__asyn(AsyncAction1)));

   using Concurrent =
      __concurrent(
        ProcedureAction1,
        ProcedureAction2);

1.x:
  ``2007 ns``

2.0:
  ``108 ns``


如果我们再增加一个并发：

.. code-block:: c++

   using ProcedureAction3 =
      __procedure(
         __wait(3),
         __finally(__asyn(AsyncAction4)));

   using Concurrent2 =
     __concurrent(
       ProcedureAction1,
       ProcedureAction2,
       ProcedureAction3);

1.x:
  ``3338 ns``

2.0:
  ``199 ns``

如果我们将之前的顺序过程和并发过程混合在一起：

.. code-block:: c++

   using Proc = __procedure
      ( __sequential
          ( __wait(1)
          , __wait(2)
          , __wait(3)
          , __wait(4)
          , __wait(5)
          , __wait(6)
          , Concurrent2),
        __finally(__sequential(__wait(7), __wait(8), __wait(9)))
      );

1.x:
  ``5402 ns``

2.0:
  ``338 ns``

