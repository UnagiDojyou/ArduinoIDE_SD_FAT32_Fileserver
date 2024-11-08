import requests
from bs4 import BeautifulSoup
import os
import time
import sys

ipaddr = '192.168.0.0'

url = 'http://' + ipaddr

def set_ipaddr(ip):
    global ipaddr, url
    ipaddr = ip
    url = 'http://' + ipaddr

def get_file_dir(dir, retry = 5, timeout = (6, 30), wait_time = 5):
    if not dir[0] == '/':
        dir = '/' + dir
    for i in range(retry):
        try:
            response = requests.get(url + dir, timeout=timeout)
        except requests.ConnectionError:
            print('conecction Error in get_file_dir. try:' + str(i + 1), file=sys.stderr)
            time.sleep(wait_time)
            continue
        except requests.Timeout:
            print('timeout in get_file_dir. try:' + str(i + 1), file=sys.stderr)
            time.sleep(wait_time)
            continue
        response.encoding = 'utf-8'
        soup = BeautifulSoup(response.text, 'html.parser')
        FileAndDirList = [link.get_text() for link in soup.find_all('a')]
        if dir == '/':
            return FileAndDirList[:-1] #delete 'ArduinoIDE_SdFat_FAT32_Fileserver'
        else:
            return FileAndDirList[1:-1] #delete 'Parent Directory' 'ArduinoIDE_SdFat_FAT32_Fileserver'
    print('All try was failed', file=sys.stderr)
    return False


def check_path_exist(path, retry = 5, timeout = (6, 30), wait_time = 5):
    if not path[0] == '/':
        path = '/' + path
    for i in range(retry):
        try:
            response = requests.get(url + path, timeout=timeout)
        except requests.ConnectionError:
            print('conecction Error in check_path_exist. try:' + str(i + 1), file=sys.stderr)
            time.sleep(wait_time)
            continue
        except requests.Timeout:
            print('timeout in check_path_exist. try:' + str(i + 1), file=sys.stderr)
            time.sleep(wait_time)
            continue
        response.encoding = 'utf-8'
        if response.status_code == 200:
            return True
        elif response.status_code == 404:
            return False
        else:
            print('unexpected response status. try:' + str(i + 1), file=sys.stderr)
            continue
    return False


def delete_file_dir(path, retry = 5, timeout = (6, 30), wait_time = 5):
    if not path[0] == '/':
        path = '/' + path
    if path[-1] == '/':
        if not len(get_file_dir(path)) == 0:
            print('directory is not empty. cannot delete directory.')
        data = {
        'cmdfilename': os.path.basename(path[:-1]) + '/',
        'delete': 'delete'
        }
        url_path = url + os.path.dirname(path[:-1]) + '/'
    else:
        data = {
        'cmdfilename': os.path.basename(path),
        'delete': 'delete'
        }
        url_path = url + os.path.dirname(path) + '/'
    for i in range(retry):
        try:
            response = requests.post(url_path, data=data, timeout=timeout)
        except requests.ConnectionError:
            print('conecction Error in delete_file. try:' + str(i + 1), file=sys.stderr)
            time.sleep(wait_time)
            continue
        except requests.Timeout:
            print('timeout in delete_file. try:' + str(i + 1), file=sys.stderr)
            time.sleep(wait_time)
            continue
        if check_path_exist(path):
            print('File was not deleted. try:' + str(i + 1), file=sys.stderr)
            continue
        else:
            return True
    print('All try was failed', file=sys.stderr)
    return False

# recursive delete
def delete_dir_r(dir):
    if not dir[0] == '/':
        dir = '/' + dir
    if not dir[-1] == '/':
        print(dir + 'is not directory', file=sys.stderr)
        return False
    dir_list = get_file_dir(dir)
    for content in dir_list:
        if content[-1] == '/':
            if not delete_dir_r(dir + content):
                print('fail to delete ' + dir + content, file=sys.stderr)
                return False
        else:
            if not delete_file_dir(dir + content):
                print('fail to delete ' + dir + content, file=sys.stderr)
                return False
    return delete_file_dir(dir)

def upload_file(local_file_path, upload_dir, retry = 5, timeout = (6, 30), wait_time = 5):
    if not os.path.exists(local_file_path):
        print("file not found", file=sys.stderr)
        return False
    if not upload_dir[0] == '/':
        upload_dir = '/' + upload_dir
    if not upload_dir[-1] == '/':
        upload_dir = upload_dir + '/'
    if not check_path_exist(upload_dir):
        if not make_dir(upload_dir):
            print('could not make parent dir in upload_file.', file=sys.stderr)
            return False
    for i in range(retry):
        file = open(local_file_path, 'rb')
        try:
            files = {'uploadfile': (os.path.basename(local_file_path), file, 'multipart/form-data')}
            response = requests.post(url + upload_dir, files=files, timeout=timeout)
        except requests.ConnectionError:
            print('conecction Error in upload_file. try:' + str(i + 1), file=sys.stderr)
            time.sleep(wait_time)
            file.close
            delete_file_dir(upload_dir + os.path.basename(local_file_path))
            continue
        except requests.Timeout:
            print('timeout in upload_file. try:' + str(i + 1), file=sys.stderr)
            time.sleep(wait_time)
            file.close
            delete_file_dir(upload_dir + os.path.basename(local_file_path))
            continue
        if check_path_exist(upload_dir + os.path.basename(local_file_path)):
            return True
        else:
            print(upload_dir + os.path.basename(local_file_path))
            print('uploaded but not found the file. try:' + str(i + 1), file=sys.stderr)
            continue
    print('All try was failed', file=sys.stderr)
    return False

# make directory. parent_dir/newdir_name/
def make_dir(path, retry = 5, timeout = (6, 60), wait_time = 5):
    if not path[0] == '/':
        path = '/' + path
    if not path[-1] == '/':
        path = path + '/'
    if check_path_exist(path):
        return True # directory is exist
    parent_dir = os.path.dirname(path[:-1]) + '/'
    newdir_name = os.path.basename(path[:-1])
    if check_path_exist(parent_dir):
        if not make_dir(parent_dir):
            print('could not make parent dir in make_dir.', file=sys.stderr)
            return False
    data = {
    'cmdfilename': newdir_name,
    'mkdir': 'mkdir'
    }
    for i in range(retry):
        try:
            response = requests.post(url + parent_dir, data=data, timeout=timeout)
        except requests.ConnectionError:
            print('conecction Error in make_dir. try:' + str(i + 1), file=sys.stderr)
            time.sleep(wait_time)
            continue
        except requests.Timeout:
            print('timeout in make_dir. try:' + str(i + 1), file=sys.stderr)
            time.sleep(wait_time)
            continue
        if check_path_exist(parent_dir + '/' + newdir_name + '/'):
            return True
        else:
            print('Directory was not made. try:' + str(i + 1), file=sys.stderr)
            continue
    print('All try was failed', file=sys.stderr)
    return False
    
# change file or directory name. old_path and new_path have to be same parent directories.
def change_name(old_path, new_path, retry=5, timeout = (6, 30), wait_time = 5):
    if old_path == new_path:
        return True
    if not old_path[0] == '/':
        old_path = '/' + old_path
    if not new_path[0] == '/':
        new_path = '/' + new_path
    if old_path[-1] == '/' and new_path[-1] == '/':
        if not os.path.dirname(old_path[:-1]) == os.path.dirname(new_path[:-1]):
            print('old_path and new_path have different parent directories.', file=sys.stderr)
            return False
        parent_path = os.path.dirname(new_path[:-1]) + '/'
        old_name = os.path.basename(old_path[:-1]) + '/'
        new_name = os.path.basename(new_path[:-1]) + '/'
    elif (not old_path[-1] == '/') and (not new_path[-1] == '/'):
        if not os.path.dirname(old_path) == os.path.dirname(new_path):
            print('old_path and new_path have different parent directories.', file=sys.stderr)
            return False
        parent_path = os.path.dirname(new_path) + '/'
        old_name = os.path.basename(old_path)
        new_name = os.path.basename(new_path)
    else:
        print('old_path or new_path is worng.', file=sys.stderr)
        return False
    if not check_path_exist(old_path):
        print('old_path is not exist.', file=sys.stderr)
        return False
    if check_path_exist(new_path):
        print('new_path is exist.', file=sys.stderr)
        return False
    data = {
    'cmdfilename': old_name,
    'newfilename': new_name,
    'rename': 'rename'
    }
    for i in range(retry):
        try:
            response = requests.post(url + parent_path, data=data, timeout=timeout)
        except requests.ConnectionError:
            print('conecction Error in change_name. try:' + str(i + 1), file=sys.stderr)
            time.sleep(wait_time)
            continue
        except requests.Timeout:
            print('timeout in change_name. try:' + str(i + 1), file=sys.stderr)
            time.sleep(wait_time)
            continue
        response.encoding = 'utf-8'
        if not '<input type="submit" name="rename" value="rename">' in response.text:
            print('rename is not support', file=sys.stderr)
            return False
        elif check_path_exist(old_path):
            if check_path_exist(new_name):
                print('file was accidentally copied.')
                return False
            print('could not change name. try:' + str(i + 1), file=sys.stderr)
            continue
        elif check_path_exist(new_path):
            return True
    print('All try was failed', file=sys.stderr)
    return False


if __name__=="__main__":
    print('enter IP address')
    set_ipaddr(input())
    print('ArduinoIDE SD FAT32 Fileserver API')
    if not (check_path_exist('/')):
        print(ipaddr + 'not available')
        sys.exit(1)
    print('ls /')
    print(get_file_dir('/'))
    print()

    print('mkdir /APItest/')
    if not make_dir('/APItest/'):
        print('fail to make directory /testAPI')
        sys.exit(1)
    print(get_file_dir('/'))
    print()
    
    print('upload hello.txt')
    path = './hello.txt'
    f = open(path, 'w')
    f.write('hello word')
    f.close()
    if not upload_file('./hello.txt', '/APItest/'):
        print('fail to upload hello.txt')
        sys.exit(1)
    print('ls /APItest/')
    print(get_file_dir('/APItest/'))
    print()
    
    print('cat /APItest/hello.txt')
    response = requests.get(url + '/APItest/hello.txt', timeout=(6, 30))
    print(response.text)
    print()

    print('rename hello.txt -> word.txt')
    if not change_name('/APItest/hello.txt', '/APItest/word.txt'):
        print('fail to rename')
        sys.exit(1)
    print('ls /APItest/')
    print(get_file_dir('/APItest/'))
    print()

    print('rename /APItest/ -> /APItested/')
    if not change_name('/APItest/', '/APItested/'):
        print('fail to rename')
        sys.exit(1)
    print('ls /')
    print(get_file_dir('/'))
    print('ls /APItested/')
    print(get_file_dir('/APItested/'))
    print()

    print('Recursive directory make')
    if not upload_file('./hello.txt', '/API_R_test/test1/test2/test3/test4/'):
        print('fail to upload')
        sys.exit(1)
    print('ls /API_R_test/test1/test2/test3/test4/')
    print(get_file_dir('/API_R_test/test1/test2/test3/test4/'))
    print()
    
    print('delete')
    os.remove(path)
    if not delete_file_dir('/APItested/word.txt'):
        print('cannot delte /APItested/word.txt')
        sys.exit(1)
    if not delete_file_dir('/APItested/'):
        print('cannot delete /APItested/')
        sys.exit(1)
    if not delete_dir_r('/API_R_test/'):
        print('cannot delete /API_R_test/')
        sys.exit(1)
    print()

    print('ls /')
    print(get_file_dir('/'))

