item_list = []
for i in range(0,3):
    a = input("Enter a number:")
    item_list.append(int(a))

item_list.sort()
item_list.reverse()
for item in item_list:
    print(item,end = " ")
