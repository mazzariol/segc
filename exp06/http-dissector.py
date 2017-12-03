import getopt
import pyshark
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
from scapy.all import *

__author__ = 'guilherme.mazzariol e marcelo.machado'


def run(filename):
    try:
        cap = pyshark.FileCapture(filename)
        streams = {}
        for pkt in cap:
            try:
                if 'HTTP' in str(pkt.layers):
                    if pkt.tcp.stream in streams.keys():
                        streams[pkt.tcp.stream] += pkt
                    else:
                        streams[pkt.tcp.stream] = [pkt]
            except:
                pass

        data = []
        for key in streams.keys():
            for pkt in streams[key]:
                try:
                    if 'data' in str(pkt.layer_name):
                        data.append(pkt.tcp_reassembled_data)
                except:
                    pass


        count = 1
        for item in data:
            bin_file = filename.replace(filename.split('/')[-1], "{0}.bin".format(count))
            file = open(bin_file, 'w')
            file.write(item)
            file.close()
            count += 1
    except:
        pass


def main(argv):
    try:
        opts, args = getopt.getopt(argv, "r:")
    except getopt.GetoptError:
        sys.exit(2)
    for opt, arg in opts:
        if opt in '-r':
            run(arg)


if __name__ == '__main__':
    main(sys.argv[1:])
