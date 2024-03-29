compiler = gcc
objs_em = event_manager.o date.o event.o event_manager_tests.o member.o priority_queue.o
objs_pq = priority_queue.o priority_queue_tests.o
COMP_FLAG = -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG

event_manager: $(objs_em)
	$(compiler) $(objs_em) -o $@
priority_queue: $(objs_pq)
	$(compiler) $(objs_pq) -o $@
date.o: date.c date.h
	$(compiler) -c $(COMP_FLAG) $*.c
event.o: event.c event.h priority_queue.h date.h member.h
	$(compiler) -c $(COMP_FLAG) $*.c
event_manager.o: event_manager.c event.h priority_queue.h date.h member.h event_manager.h
	$(compiler) -c $(COMP_FLAG) $*.c
event_manager_tests.o: tests/event_manager_tests.c tests/test_utilities.h event_manager.h date.h
	$(compiler) -c $(COMP_FLAG) tests/event_manager_tests.c
member.o: member.c member.h
	$(compiler) -c $(COMP_FLAG) $*.c
priority_queue_tests.o: tests/priority_queue_tests.c tests/test_utilities.h priority_queue.h
	$(compiler) -c $(COMP_FLAG) tests/priority_queue_tests.c
priority_queue.o: priority_queue.c priority_queue.h
	$(compiler) -c $(COMP_FLAG) $*.c
clean: 
	rm -rf *o event_manager priority_queue