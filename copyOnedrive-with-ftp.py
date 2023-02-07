import os
from ftplib import FTP_TLS

host = '127.0.0.1'
port = 21

usr = "username"
pwd = "password"

osName = os.getlogin()

# Local get path
dir = "C:\\Users\OSAL0401\\OneDrive - IT-Enheten SML" 


ftp = FTP_TLS()

ftp.connect(host=host, port=port)

ftp.login(user=usr, passwd=pwd)

ftp.prot_p()

print(ftp.pwd())

def send_file(ftp, localFilePath, onlineFilePath):
    with open(localFilePath, "rb") as file: 
        ftp.storbinary(onlineFilePath, file)


def change_slash(string):
    returnString = ""
    for num in range(len(string)):
        char = string[num]
        if char == "\\":
            try:
                if string[num - 1] == "/":
                    continue
            except:
                pass
            char = "/"
    
        returnString += char
    return returnString

for path, subdirs, files in os.walk(dir):
    for fileName in files:

        # Original file path
        filePath = change_slash(os.path.join(path, fileName))

        # File path to the place that the file is going to be copied to
        storeFilePath = change_slash((osName + filePath[len(dir):]))

        # Dir path to the file that is going to be copied
        dirPath = change_slash(storeFilePath[:len(storeFilePath) - len(fileName)]) 

        try:
            ftp.mkd(change_slash(dirPath))
        except:
            pass
        
        print("Uploadning: " + filePath + " to: " + storeFilePath)
        with open(filePath, "rb") as file: 
            ftp.storbinary("STOR " + storeFilePath, file)
        print("Done")


print('All work completed')
ftp.quit()
