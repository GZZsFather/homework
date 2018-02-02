import os
import sys
import pcap
import dpkt
import time

from Tkinter import *
from scapy.all import *
from ScrolledText import ScrolledText

count = 1
index = 0
p_num = 10
device = 'en0'
soft_name = "rpt_shark version3.0 beta"

def gettime():
    global localtime
    localtime = time.asctime( time.localtime(time.time()))
    return localtime

def PacHandler(pkt):
    # pkt.summary()
    global count
    t.insert(END, str(count)+ " : ")
    count = count + 1
    t.insert(END,str(pkt.summary())+"\n")
    t.update()
    t.see(END)

#def test():
#    print 'test'


def sniff_show():
    index = entryname3.get()
    if(index == ""):
        index = 1
    t2.delete(1.0,END)
    item = dpkt[int(index)-1]
    cache_write(item)
    # t2.insert(END, dpkt[int(index)-1].show)
    t2.update()
    # t2.see(END)

def cache_write(item):
    f = open("cache","w+")
    old_out = sys.stdout
    sys.stdout = f
    print item.show()
    sys.stdout = old_out
    f.close()

    f=open("cache","r+")
    data = f.readlines()
    for item in data:
        t2.insert(END,item)
    f.close()
    os.remove("cache")

def sniff_start():
    global count
    count = 1
    global dpkt

    p_num = entryname.get()
    if(p_num == ""):
        p_num = 10

    dpkt  = sniff(filter = "ip",iface = "en0", count = int(p_num) , prn = PacHandler)
    t.insert(END,"\n\n------------------------------Sniff finished------------------------------\n\n\n")
    t.see(END)
    t3.insert(END,str(gettime()) + '\n')
    t3.insert(END,str(dpkt)+"\n")
    t3.see(END)



def export_log():
    name = localtime+'.log'
    f = open(name ,'w+')
    old_out = sys.stdout
    sys.stdout = f
    for item in dpkt:
        print item.show()
    sys.stdout = old_out
    f.close()
    wrpcap(localtime + '.pcap',dpkt)



root = Tk()
root.title(soft_name)
root.geometry("1000x800")
# root.resizeable(width = True, height = True)
t = ScrolledText(root)
t2 = ScrolledText(root)
t3 = Text(root)

entryname = Entry(root,width = 80, textvariable=p_num)
entryname.place(x=770,y=10,width = 100,height = 20)
entryname2 = Entry(root,width = 80, textvariable=device)
entryname2.place(x=770,y=30,width = 100,height = 20)
entryname3 = Entry(root,width = 80, textvariable=index)
entryname3.place(x=770,y=220,width = 100,height = 20)

labelName = Label(root,text='p_num(default:10): ',justify = RIGHT,width = 10 )
labelName.place(x=640,y=10,width=130,height=20)
labelName2 = Label(root,text='       device : ',justify = RIGHT,width = 10 )
labelName2.place(x=640,y=30,width=130,height=20)
labelName3 = Label(root,text='index(default:1 ): ',justify = RIGHT,width = 10 )
labelName3.place(x=640,y=220,width=130,height=20)

buttonOk = Button(root,text = 'sniff',command = sniff_start)
buttonOk.place(x=700,y=60,width = 50,height = 20)
buttonOk1 = Button(root,text = 'details',command = sniff_show)
buttonOk1.place(x=880,y=220,width = 80,height = 20)
buttonOk2 = Button(root,text = 'export',command = export_log)
buttonOk2.place(x=780,y=60,width = 70,height = 20)

t.insert('2.0', soft_name+"\n")
t2.insert('2.0', soft_name+"\n")
t3.insert('2.0', soft_name+"\n")
t.place(x=20, y=20, width=600, height=700)
t2.place(x=650, y = 250, width = 350, height = 500)
t3.place(x =650, y = 100, width = 340, height = 120)

root.mainloop()
