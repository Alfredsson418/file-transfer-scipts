from queue import Queue
import threading, os

q = Queue()

osName = os.getlogin()
storePath = "C:\\ElevProgram\\OneDrive\\" + osName + "\\"
dir = "C:\\Users\\" + osName + "\\OneDrive - IT-Enheten SML\\"



def create_file(filePath, binary):

    with open(filePath, "wb") as file: 
        file.write(binary)



def copy_file(filePath, storeFilePath):
    binary = 0
    
    try:    
        with open(filePath, "rb") as file:
            binary = file.read()
    except PermissionError:
        return

    create_file(storeFilePath, binary)



def worker():
    while True:

        item = q.get()
        func = item[0]
        args = item[1:]
        func(*args)

        q.task_done()

        


for path, subdirs, files in os.walk(dir):
    for fileName in files:
        # Original file path
        filePath = os.path.join(path, fileName) 

        # File path to the place that the file is going to be copied to
        storeFilePath = (storePath + filePath[len(dir):]) 

        # Dir path to the file that is going to be copied
        dirPath = storeFilePath[:len(storeFilePath) - len(fileName)] 

        # Creates dirs if it does not exist
        if not os.path.isdir(dirPath[:len(dirPath)-1]):
            os.makedirs(dirPath[:len(dirPath)-1])

        q.put( (copy_file, filePath, storeFilePath) , block=False, timeout=10)


# Turn-on the worker thread.
threading.Thread(target=worker, daemon=True).start()


# Block until all tasks are done.
q.join()
# All work is complete
