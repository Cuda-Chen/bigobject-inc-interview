# q2-solution

[8f9935a](https://github.com/Cuda-Chen/bigobject-inc-interview/commit/ed4afadc11a6dbee2977c2c7a370e2d42b859370)
使用 Thread Sanitizer 可以發現有 deadlock 之問題：

```bash
$ g++ -o q2 q2.cpp -Wall -g -lpthread -fsanitize=thread
$ ./q2
==================
WARNING: ThreadSanitizer: lock-order-inversion (potential deadlock) (pid=23836)
  Cycle in lock order graph: M12 (0x5593409fa060) => M11 (0x5593409fa0a0) => M12

  Mutex M11 acquired here while holding mutex M12 in thread T1:
    #0 pthread_rwlock_rdlock ../../../../src/libsanitizer/tsan/tsan_interceptors_posix.cpp:1338 (libtsan.so.0+0x3cab8)
    #1 thd_func1(void*) /home/jio/cpp_code/bigobject-inc-interview/q2.cpp:15 (q2+0x13ce)

    Hint: use TSAN_OPTIONS=second_deadlock_stack=1 to get more informative warning message

  Mutex M12 acquired here while holding mutex M11 in thread T2:
    #0 pthread_mutex_lock ../../../../src/libsanitizer/sanitizer_common/sanitizer_common_interceptors.inc:4240 (libtsan.so.0+0x53908)
    #1 __gthread_mutex_lock /usr/include/x86_64-linux-gnu/c++/11/bits/gthr-default.h:749 (q2+0x16e2)
    #2 std::mutex::lock() /usr/include/c++/11/bits/std_mutex.h:100 (q2+0x176a)
    #3 thd_func2(void*) /home/jio/cpp_code/bigobject-inc-interview/q2.cpp:24 (q2+0x1480)

  Thread T1 (tid=23838, finished) created by main thread at:
    #0 pthread_create ../../../../src/libsanitizer/tsan/tsan_interceptors_posix.cpp:969 (libtsan.so.0+0x605b8)
    #1 main /home/jio/cpp_code/bigobject-inc-interview/q2.cpp:36 (q2+0x1550)

  Thread T2 (tid=23839, running) created by main thread at:
    #0 pthread_create ../../../../src/libsanitizer/tsan/tsan_interceptors_posix.cpp:969 (libtsan.so.0+0x605b8)
    #1 main /home/jio/cpp_code/bigobject-inc-interview/q2.cpp:37 (q2+0x1571)

SUMMARY: ThreadSanitizer: lock-order-inversion (potential deadlock) /home/jio/cpp_code/bigobject-inc-interview/q2.cpp:15 in thd_func1(void*)
==================
ThreadSanitizer: reported 1 warnings
```

在看程式碼（q2.cpp）之後可以發現，`thd_func2` 的 lock 順序應該進行調整：

```cpp
void *thd_func2 (void *arg) {
    pthread_rwlock_wrlock(&rwlock);
    counter_mtx.lock();
    counter--;
    counter_mtx.unlock();
    pthread_rwlock_unlock(&rwlock);
    return NULL;
}
```

對於 lock 順序，應該先 lock `counter_mtx` -> write lock `rwlock` -> unlock `rwlock` -> unlock `counter_mtx`。

相關 changes 可以在 [acb685a](https://github.com/Cuda-Chen/bigobject-inc-interview/commit/acb685aa5bf9d702e44e264f76d97fa19f1df4ed) 看到。
