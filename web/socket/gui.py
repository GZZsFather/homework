from tkinter import *
def hello():
    print("Hello world!")

win = Tk()
win.title("Hello World!")
win.geometry('400x200')

btn = Button(win,text='Click me',command = hello)

btn.pack(expand = YES, fill = BOTH)

mainloop()
