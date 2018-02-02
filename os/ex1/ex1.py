class PCB:
    def __init__(self,ID,Priority,arrival,split_needed):
        self.ID = ID
        self.PID = 0                                #进程标识数
        self.Next = 0                               #链指针
        self.Priority = Priority                    #优先级
        self.arrival = arrival                      #到达时间
        self.occupied_split = 0                     #占用 CPU 时间片数
        self.split_needed = split_needed            #进程所需时间片数
        self.status = 'W'                           #进程状态

def show_all(obj):
    print('%10s %10s %10s %10s %10s %10s %10s'%('ID', 'Next', 'priority', 'arrival',
        'occupied', 'split_needed', 'status'))
    for item in obj:
        print('%10s %10s %10s %10s %10s %10s %10s '%(item.ID, item.Next, item.Priority, item.arrival,
            item.occupied_split, item.split_needed, item.status))

def list_show(waiting):
    print('waiting: ',end = '')
    for item in waiting:
        print(item.ID,end = ' ')
    print('')
    if len(waiting) == 0:
        print('')

def find_HEAD_TAIL(waiting):
    list_show(waiting)
    if len(waiting) == 0:
        HEAD = None
        TAID = None
    else:
        HEAD = waiting[-1]
        TAID = waiting[0]

    return HEAD,TAID

def find_NEXT(TAID,waiting,RUN,HEAD):
    if RUN != None and HEAD != None:
        RUN.Next = HEAD.ID
    if TAID != None:
        TAID.Next = -1
    for i in range((len(waiting)-1),0,-1):
        print('')
        waiting[i].Next = waiting[i-1].ID


def Priority_Execute(PCB):
    # return 1 if finished
    PCB.Priority = PCB.Priority - 3
    PCB.occupied_split = PCB.occupied_split + 1

    #你要是问我下面这两行代码是干啥的，emmm 是用来防止出bug以后出不了循环的
    if PCB.occupied_split > 20:
        exit(0)

    if PCB.occupied_split == PCB.split_needed:
        PCB.status = 'F'
        return 1
    else:
        return 0


def Priority(waiting,finished,proc,RUN,HEAD,TAID):
    waiting = sorted(proc, key = lambda x : x.Priority)
    # 这一部分是强制重置未完成的任务的状态
    for item in waiting:
        if item.status != 'F':
            item.status = 'W'

    if RUN == None:
        RUN = waiting.pop()
        RUN.status = 'R'
        HEAD,TAID = find_HEAD_TAIL(waiting)

        if HEAD != None:
            print('HEAD = %d'%HEAD.ID)
        if TAID != None:
            print('TAID = %d'%TAID.ID)
        if RUN != None:
            print('RUN = %d'%RUN.ID)


    result = Priority_Execute(RUN)
    find_NEXT(TAID,waiting,RUN,HEAD)

    if result:
        print('%d finished'%(RUN.ID))
        RUN.status = 'F'
        proc.remove(RUN)
        finished.append(RUN)
        RUN = None
    elif HEAD != None:

        if  RUN.Priority > HEAD.Priority:
            print('%d and %d exchanged!'%(RUN.ID,HEAD.ID))
            RUN.status = 'W'
            temp = RUN
            RUN = waiting.pop()
            RUN.status = 'R'
            waiting.append(temp)
            waiting.sort(key = lambda x : x.Priority)
            HEAD,TAID = find_HEAD_TAIL(waiting)

def RR_Executue(PCB):
    PCB.occupied_split = PCB.occupied_split + 1
    if PCB.occupied_split == PCB.split_needed:
        print('%d is finished!'%PCB.ID)
        PCB.status = 'F'
        return 1
    return 0

def Round_robin(finished,proc,RUN,HEAD,TAID):
    global i
    RUN = proc[i]
    result = RR_Executue(RUN)

    if result:
        i = i + 1
        finished.append(RUN)
        RUN = None


def main():
    RUN = None
    HEAD = None
    TAID = None
    LENGTH = 0
    N = 20

    time = 0
    alll = []
# ID,Priority,arrival,split_needed
    PCB1 = PCB(1,0,2,1)
    PCB2 = PCB(2,2,1,3)
    PCB3 = PCB(3,100,3,4)
    PCB4 = PCB(4,1,2,3)

    alll.append(PCB1)
    alll.append(PCB2)
    alll.append(PCB3)
    alll.append(PCB4)
    finished = []
    waiting = []
    proc = []

    LENGTH = len(alll)
    choice = input("Choose an Algo: \n\t1.priority\n\t2.round robin\nAnd your choice is(1 or 2) : ")

    print('')
    while choice != '1' and choice !='2':
        print('You bad.\n')
        choice = input("Make your choice again: \n\t1.priority\n\t2.round robin\nAnd your choice is(1 or 2) : ")

    if choice == '1':
        print("And your choice is priority")

        while(time <= N):
            if len(finished) == len(alll):
                print('all tasks finished!')
                break
            time = time + 1
            print('Now is %d'%time)

            for item in alll:
                if item.arrival == time and item.status != 'F':
                    proc.append(item)

            Priority(waiting,finished,proc,RUN,HEAD,TAID)

            show_all(alll)


    elif choice == '2':
        print("And your choice is round robin")

        global i
        i = 0

        while(time <= N):
            if len(finished) == len(alll):
                print('all tasks finished')
                break
            time = time + 1
            print('Now is %d'%time)

            for item in alll:
                if item.arrival == time and  item.status != 'F':
                    proc.append(item)


            Round_robin(finished,proc,RUN,HEAD,TAID)

            show_all(alll)


if __name__ == '__main__':
    main()
