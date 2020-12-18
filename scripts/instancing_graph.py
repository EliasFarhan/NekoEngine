import sqlite3
import sys
import matplotlib.pyplot as plt


def main(db_path):
    conn = sqlite3.connect(db_path)
    c = conn.cursor()
    segment = 4
    instance = 10
    while segment <= 256:
        label = "Segment {}".format(segment)
        instances = []
        avgs = []
        while instance <= 100000:
            c.execute("SELECT Time, Indice FROM RenderData WHERE Instance=? AND Segment=?", (instance, segment))
            values = c.fetchall()

            sum = 0.0
            for v in values:
                sum += v[0]
            if len(values) != 0:
                indices = values[0][1]
                sum /= len(values)
                print("{} instances, {} indices, avg {}".format(instance, indices, sum))
                instances.append(instance)
                avgs.append(sum)
            instance *= 10

        plt.plot(instances, avgs, label=label)
        segment *= 8
        instance = 10
    plt.plot([0, 100000], [16666, 16666], label="60 fps")
    plt.xlabel("Instance")
    plt.xscale("log")
    plt.ylabel("Time (us)")
    plt.yscale("log")
    plt.legend()
    plt.show()

if __name__ == '__main__':
    if len(sys.argv) > 1:
        main(sys.argv[1])
    else:
        sys.stderr.write("Error not enough argument\n")
