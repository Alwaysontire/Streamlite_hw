from multiprocessing import Process

def print_task(n):
    print(f"процесс {n} начался")
    time.sleep()
    print(f"процесс {n} завершился")

procesesse = [Process(target=print_task, args=(i,)) for i in range(3)]
for p in procesesse:
    p.start()
for p in procesesse:
    p.join()