import subprocess

def main():
    alpha = 0.1
    epsilon = 0.2
    gamma = 0.99
    trial_id = 1
    # get subprocess stdout
    x = subprocess.run(["main.exe", f"{trial_id}_state.bin", str(alpha), str(epsilon), str(gamma)], capture_output=True)
    prefix = "Average reward: "
    reward = None
    for line in reversed(x.stdout.decode("ascii").splitlines()):
        if line.startswith(prefix):
            reward = float(line[len(prefix):].strip())
            break
    print(reward)

if __name__ == "__main__":
    main()
