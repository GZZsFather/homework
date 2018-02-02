"""
 给四个区域编号
 左上为1
 左下为2
 右上为3
 右下为4
 tr为当前方块左上角的横坐标
 tc为当前方块左上角的纵坐标
 pr为当前特殊点横坐标
 pc为当前特殊点纵坐标
"""

count = 1
# table = [[-1 for i in range(8)] for i in range(8)]

#在当前区域寻找需要被标为下一步特殊点的坐标
#zone为所在区域编号
def FindSpec(tr, tc, size, zone):
    size = size - 1
    if zone == 1:
        return (tr+size, tc+size)
    if zone == 2:
        return (tr+size, tc)
    if zone == 3:
        return (tr,tc+size)
    if zone == 4:
        return (tr, tc)

#寻找各个区域的起点
def FindZoneStart(tr, tc, size, zone):
    half_size = size // 2
    if zone == 1:
        return (tr, tc)
    if zone == 2:
        return (tr, tc + half_size)
    if zone == 3:
        return (tr + half_size, tc)
    if zone == 4:
        return (tr + half_size, tc + half_size)

#返回特殊点所在的区域编号
def FindZone(tr, tc, pr, pc, size):
    half_size = size // 2
    center_x = tr + half_size
    center_y = tc + half_size
    abs_x = pr - center_x
    abs_y = pc - center_y

    if abs_x < 0 :
        if abs_y < 0 :
            return 1
        else:
            return 2
    else:
        if abs_y < 0 :
            return 3
        else:
            return 4


#函数主体，递归实现
def chess(tr, tc, pr, pc, size):
    global count
    if size == 2 :
        for i in range(2):
            for j in range(2):
                curx = tr + i
                cury = tc + j
                if table[curx][cury] == 0 :
                    table[curx][cury] = count
        count = count + 1
    else :
        spec_zone = FindZone(tr, tc, pr, pc, size)
        half_size = size // 2
        for i in range(1,5):
            if i != spec_zone:
                # print("executing ",i)
                new_tr,new_tc =FindZoneStart(tr, tc, size, i)
                spec_x,spec_y = FindSpec(new_tr, new_tc, half_size, i)
                table[spec_x][spec_y] = count

        # print(count)
        # show(table)
        count = count + 1

        for i in range(1,5):
            new_tr,new_tc =FindZoneStart(tr, tc, size, i)
            spec_x,spec_y = FindSpec(new_tr, new_tc, half_size, i)
            chess(new_tr, new_tc, spec_x, spec_y, half_size)

def show(table):
    n=len(table)
    for i in range(n):
        for j in range(n):
            if table[i][j] != -1:
                print('%3d'%table[i][j],end='  ')
            else:
                print('  #',end = '  ')
        print('')
    print('')

def chess_cover(tr, tc, pr, pc, size):
    chess(tr, tc, pr, pc, size)
    table[pr][pc] = -1

if __name__ == "__main__":
    count = 1
    case = 1
    while(1):

        k,x,y = map(int,input().split())
        n = 2**k
        x = x - 1
        y = y - 1
        table = [[0 for i in range(int(n))] for i in range(int(n))]
        chess_cover(0,0,y,x,int(n))
        print("Case %d: n = %d"%(case,n))
        case = case + 1
        show(table)
