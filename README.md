


## Что это?

STL-совместимый циклический буфер CCircularBuffer.

[Циклический буфер](https://en.wikipedia.org/wiki/Circular_buffer) - структура данных, которая часто применяется для буферизации в задачах по передаче и обработке потока данных.



## Что умеет?

- Соответствует [требованиям](https://en.cppreference.com/w/cpp/named_req/Container) для stl-контейнера.
- Соответствует [требованиям](https://en.cppreference.com/w/cpp/named_req/SequenceContainer) для последовательного контейнера. Без rvalue и move-семантики...
- Буфер параметризируется типом хранимого значения и аллокатором.
- Предоставляет итератор произвольного доступа. Итератор соответствует [требованиям](https://en.cppreference.com/w/cpp/named_req/RandomAccessIterator).
- Реализация кольцевого буфера так же покрыта тестами, с помощью фреймворка Google Test.



## Как использовать?

Подключить заголовочный файл lib/CCircularBuffer.h
