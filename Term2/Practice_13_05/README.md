# Practice 13 May: Yandex Office Tour

## Author
Egor Kadomtsev, Group 25.B81-mm

## Description

The console program models an office tour with the required `Room` and `Guide` classes. Five rooms
are allocated with `new`, stored in a `Room* rooms[5]` array, accessed through pointers, and deleted
before exit. The menu supports visits, current-room details, all visited rooms, and the most visited
room.

## Build and run

```bash
make
./bin/office_tour
```

## Test

```bash
make test
make sanitize
```
