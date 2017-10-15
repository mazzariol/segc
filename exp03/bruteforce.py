#!/usr/bin/python

import sys
import getopt
from zipfile import ZipFile

__author__ = 'guilherme.mazzariol-RA-138466 && marcelo.machado-RA-210402'


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
    except Exception:
        pass
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
            count = len(list_pwd) - 1
            while count > 0 and password == '':
                count -= 1
                try:
                    zf.extractall(path='/tmp/', pwd=list_pwd[0].encode('utf-8'))
                    password = list_pwd[0]
                except Exception:
                    list_pwd.remove(list_pwd[0])
        zf.close()
    except Exception:
        pass
    finally:
        return password


def main(argv):
    source_list = ''
    zip_filename = ''
    try:
        opts, args = getopt.getopt(argv, "hl:f:v", ["help, list, file"])
    except getopt.GetoptError:
        sys.exit(2)
    else:
        if len(opts) == 2:
            for opt, arg in opts:
                if opt in ('-l', '--list'):
                    source_list = arg
                elif opt in ('-f', '--file'):
                    zip_filename = arg
                else:
                    print('Unknown parameter!\n')
                    sys.exit(2)
            lines = read_file(source_list)
            pwd = access_zip_file_with_password(zip_filename, lines)
            if len(pwd) > 0:
                print('The password is {0}'.format(pwd))

if __name__ == '__main__':
    main(sys.argv[1:])
