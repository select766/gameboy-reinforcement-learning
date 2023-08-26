# printfで使うフォントとの干渉を避けるために、bg_map.cのタイル番号に128を足す

import argparse
import re

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('input', help='input file')
    parser.add_argument('output', help='output file')
    parser.add_argument('offset', help='offset', type=int)
    args = parser.parse_args()

    with open(args.input, 'r', encoding='utf-8') as f:
        txt = f.read()

    # replace txt: find hex value such as 0xAB, then add 10
    txt = re.sub(r'0x([0-9A-Fa-f]{2})', lambda m: '0x{:02X}'.format(int(m.group(1), 16) + args.offset), txt)

    with open(args.output, 'w', encoding='utf-8') as f:
        f.write(txt)


if __name__ == '__main__':
    main()
