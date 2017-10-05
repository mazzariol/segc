import getopt
from zipfile import ZipFile

import sys

__author__ = 'guilherme.mazzariol && marcelo'


def read_file(src_file):
    """
    Read lines from source file.
    Return a list with all lines from file.
    :param src_file:
    :return: list
    """
    lines = []
    try:
        file = open(src_file, 'r')
        lines = file.read().splitlines()
        file.close()
    except Exception as e:
        print(e)
    finally:
        return lines


def access_zip_file_with_password(zip_file, list_pwd):
    """
    Try to discorvery a zip file password from a list
    :param zip_file: source for a zip file
    :param list_pwd: list of string
    :return: string
    """
    password = ''
    try:
        with ZipFile(zip_file) as zf:
            data = 'Bad password for file'
            while list_pwd or data is not None:
                try:
                    password = list_pwd[0]
                    data = zf.extractall(path='/tmp/', pwd=password.encode('utf-8'))
                    list_pwd.remove(password)
                except Exception as e:
                    list_pwd.remove(password)
                    data = e.args[0]
        zf.close()
    except Exception as e:
        print(e)
    finally:
        return password

def main(argv):
    source_list = ''
    zip_filename = ''
    try:
        opts, args = getopt.getopt(argv, "h:l:f", ["help, list, file"])
    except getopt.GetoptError as e:
        print('bruteforce -l /tmp/list.txt -f /tmp/secrets.zip. ')
        sys.exit(2)
    else:
        for opt, arg in opts:
            if opt == '-h':
                print('help')
                sys.exit()
            elif opt in ('-l', '--list'):
                source_list = arg
            elif opt in ('-f', '--file'):
                zip_filename = arg
            else:
                print('Unknown parameter!\n')
                sys.exit(2)
        lines = read_file(source_list)
        pwd = access_zip_file_with_password(zip_filename, lines)
        print('The password is {0}'.format(pwd))

if __name__ == '__main__':
    main(sys.argv)

