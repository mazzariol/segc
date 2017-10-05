from zipfile import ZipFile

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


def extract_data(zip_file, list_pwd):
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


if __name__ == '__main__':
    src = '/home/guilherme/teste/t1.txt'
    zip = '/home/guilherme/teste/segc.zip'
    lines = read_file(src)
    pwd = extract_data(zip, lines)
    print('The password is {0}'.format(pwd))

