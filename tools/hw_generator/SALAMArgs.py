import argparse

def HWArgs():
    parser = argparse.ArgumentParser(description='Parse command line args for hardware generator')
    parser.add_argument('-b', '--bench', type=str, required=True, help='Name of benchmark')
    return parser.parse_args()