#!/usr/bin/python3.10
import csv
import os
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

class BenchmarkPlotter:
    def __init__(self, data_file='results.csv', plot_dir='plots'):
        self.data_file = data_file
        self.plot_dir = plot_dir
        os.makedirs(self.plot_dir, exist_ok=True)
        self.records = self._read_data()

    def _read_data(self):
        rows = []
        if not os.path.exists(self.data_file):
            raise FileNotFoundError(f"Cannot find {self.data_file}")
        with open(self.data_file, 'r', encoding='utf-8') as f:
            reader = csv.DictReader(f)
            for r in reader:
                rows.append(r)
        return rows

    def render_all(self):
        algorithms = [
            ("Bubble Sort", "bubble_sort_performance"),
            ("Insertion Sort", "insertion_sort_performance"),
            ("Merge Sort", "merge_sort_performance"),
            ("Quick Sort (First Pivot)", "quicksort_first_pivot_performance"),
            ("Quick Sort (Random Pivot)", "quicksort_random_pivot_performance"),
            ("Quick Sort (Median of 3)", "quicksort_median3_pivot_performance"),
            ("Heap Sort", "heap_sort_performance"),
            ("Radix Sort", "radix_sort_performance")
        ]

        style_config = {
            "Random": {"color": "#4b0082", "marker": "D", "linestyle": "-"},
            "Increasing": {"color": "#008080", "marker": "s", "linestyle": "--"},
            "Decreasing": {"color": "#c71585", "marker": "^", "linestyle": "-."}
        }

        generated = []

        for algo_name, filename_stem in algorithms:
            fig, ax = plt.subplots(figsize=(8.2, 5.4), dpi=300)

            for dist_name, style in style_config.items():
                xs = []
                ys = []

                for row in self.records:
                    if row['Algorithm'] == algo_name and row['InputType'] == dist_name:
                        t_str = row['MeanTimeMicroseconds'].strip()
                        if t_str not in ('SKIPPED', 'N/A', ''):
                            try:
                                xs.append(int(row['Size']))
                                ys.append(float(t_str))
                            except ValueError:
                                pass

                if xs and ys:
                    xy = sorted(zip(xs, ys))
                    ax.plot(
                        [p[0] for p in xy],
                        [p[1] for p in xy],
                        label=f"{dist_name} Distribution",
                        color=style["color"],
                        marker=style["marker"],
                        linestyle=style["linestyle"],
                        linewidth=2,
                        markersize=7
                    )

            ax.set_title(f"{algo_name} — Runtime Performance Analysis", fontsize=13, fontweight='bold', pad=12)
            ax.set_xlabel("Dataset Size (n)", fontsize=11, fontweight='medium', labelpad=8)
            ax.set_ylabel("Execution Time (µs)", fontsize=11, fontweight='medium', labelpad=8)

            ax.set_xscale('log')
            ax.set_xticks([1000, 10000, 100000])
            ax.set_xticklabels(['1,000', '10,000', '100,000'])

            ax.grid(True, linestyle=":", alpha=0.6)
            ax.legend(title="Data Distribution", frameon=True, facecolor="#fafafa", edgecolor="#d0d0d0")

            plt.tight_layout()

            png_file = os.path.join(self.plot_dir, f"{filename_stem}.png")
            pdf_file = os.path.join(self.plot_dir, f"{filename_stem}.pdf")

            fig.savefig(png_file)
            fig.savefig(pdf_file)
            plt.close(fig)

            generated.append(png_file)
            generated.append(pdf_file)

        print(f"Successfully generated {len(generated)} plot files in '{self.plot_dir}/'")

if __name__ == '__main__':
    plotter = BenchmarkPlotter()
    plotter.render_all()
