# PCで学習したパラメータ(QLearningState)をGBで使える形式に変換する

import argparse

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('input', help='input file')
    parser.add_argument('output', help='output file')
    parser.add_argument('name', help='name of variable')
    args = parser.parse_args()

    with open(args.input, 'rb') as f:
        binary = f.read()

    # binary to text: 0xAB, 0xCD...
    txt = ', '.join(['0x{:02X}'.format(b) for b in binary])

    with open(args.output, 'w', encoding='utf-8') as f:
        f.write(f"""
const unsigned char {args.name}[] = {{{txt}}};
""")


if __name__ == '__main__':
    main()
