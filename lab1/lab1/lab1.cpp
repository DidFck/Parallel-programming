#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

// Base interface for import/export
class IPrintable {
public:
    virtual void Print() const = 0;
    virtual void Export(const std::string& filename) const = 0;
    virtual void Import(const std::string& filename) = 0;
};

// GPU Specifications class
class GpuSpec : public IPrintable {
private:
    std::string model;
    int cores;
    int memory;

public:
    GpuSpec(std::string model = "", int cores = 0, int memory = 0)
        : model(model), cores(cores), memory(memory) {
    }

    void Print() const override {
        std::cout << "GPU Model: " << model << ", Cores: " << cores << ", Memory: " << memory << "MB\n";
    }

    void Export(const std::string& filename) const override {
        std::ofstream out(filename, std::ios::app);
        if (out) {
            out << "GPU," << model << "," << cores << "," << memory << "\n";
            out.close();
        }
    }

    void Import(const std::string& filename) override {
        std::ifstream in(filename);
        if (in) {
            std::string line;
            while (std::getline(in, line)) {
                if (line.rfind("GPU,", 0) == 0) {
                    sscanf_s(line.c_str(), "GPU,%[^,],%d,%d", &model[0], model.size(), &cores, &memory);
                }
            }
            in.close();
        }
    }
};

// CPU Specifications class
class CpuSpec : public IPrintable {
private:
    std::string model;
    int cores;
    double frequency;

public:
    CpuSpec(std::string model = "", int cores = 0, double frequency = 0.0)
        : model(model), cores(cores), frequency(frequency) {
    }

    void Print() const override {
        std::cout << "CPU Model: " << model << ", Cores: " << cores << ", Frequency: " << frequency << "GHz\n";
    }

    void Export(const std::string& filename) const override {
        std::ofstream out(filename, std::ios::app);
        if (out) {
            out << "CPU," << model << "," << cores << "," << frequency << "\n";
            out.close();
        }
    }

    void Import(const std::string& filename) override {
        std::ifstream in(filename);
        if (in) {
            std::string line;
            while (std::getline(in, line)) {
                if (line.rfind("CPU,", 0) == 0) {
                    sscanf_s(line.c_str(), "CPU,%[^,],%d,%lf", &model[0], model.size(), &cores, &frequency);
                }
            }
            in.close();
        }
    }
};

// RAM Specifications class
class RamSpec : public IPrintable {
private:
    int size;
    double speed;

public:
    RamSpec(int size = 0, double speed = 0.0)
        : size(size), speed(speed) {
    }

    void Print() const override {
        std::cout << "RAM Size: " << size << "GB, Speed: " << speed << "MHz\n";
    }

    void Export(const std::string& filename) const override {
        std::ofstream out(filename, std::ios::app);
        if (out) {
            out << "RAM," << size << "," << speed << "\n";
            out.close();
        }
    }

    void Import(const std::string& filename) override {
        std::ifstream in(filename);
        if (in) {
            std::string line;
            while (std::getline(in, line)) {
                if (line.rfind("RAM,", 0) == 0) {
                    sscanf_s(line.c_str(), "RAM,%d,%lf", &size, &speed);
                }
            }
            in.close();
        }
    }
};

// LAN Specifications class
class LanSpec : public IPrintable {
private:
    double bandwidth;

public:
    LanSpec(double bandwidth = 0.0)
        : bandwidth(bandwidth) {
    }

    void Print() const override {
        std::cout << "LAN Bandwidth: " << bandwidth << "Gbps\n";
    }

    void Export(const std::string& filename) const override {
        std::ofstream out(filename, std::ios::app);
        if (out) {
            out << "LAN," << bandwidth << "\n";
            out.close();
        }
    }

    void Import(const std::string& filename) override {
        std::ifstream in(filename);
        if (in) {
            std::string line;
            while (std::getline(in, line)) {
                if (line.rfind("LAN,", 0) == 0) {
                    sscanf_s(line.c_str(), "LAN,%lf", &bandwidth);
                }
            }
            in.close();
        }
    }
};

// ClusterNode class
class ClusterNode : public IPrintable {
private:
    GpuSpec gpu;
    CpuSpec cpu;
    RamSpec ram;
    LanSpec lan;

public:
    ClusterNode(GpuSpec gpu, CpuSpec cpu, RamSpec ram, LanSpec lan)
        : gpu(gpu), cpu(cpu), ram(ram), lan(lan) {
    }

    void Print() const override {
        std::cout << "Cluster Node:\n";
        gpu.Print();
        cpu.Print();
        ram.Print();
        lan.Print();
    }

    void Export(const std::string& filename) const override {
        gpu.Export(filename);
        cpu.Export(filename);
        ram.Export(filename);
        lan.Export(filename);
    }

    void Import(const std::string& filename) override {
        gpu.Import(filename);
        cpu.Import(filename);
        ram.Import(filename);
        lan.Import(filename);
    }
};

// Cluster class
class Cluster : public IPrintable {
private:
    std::vector<ClusterNode> nodes;

public:
    void AddNode(const ClusterNode& node) {
        nodes.push_back(node);
    }

    void Print() const override {
        std::cout << "Cluster Configuration:\n";
        for (size_t i = 0; i < nodes.size(); ++i) {
            std::cout << "Node " << i + 1 << ":\n";
            nodes[i].Print();
        }
    }

    void Export(const std::string& filename) const override {
        std::ofstream out(filename);
        if (out) out.close(); // Clear file before appending
        for (const auto& node : nodes) {
            node.Export(filename);
        }
    }

    void Import(const std::string& filename) override {
        nodes.clear();
        // Logic to read nodes from the file can be added here.
    }
};

// Example usage
int main() {
    GpuSpec gpu("NVIDIA RTX 3080", 8704, 10240);
    CpuSpec cpu("Intel i9-12900K", 16, 3.9);
    RamSpec ram(32, 3200);
    LanSpec lan(1.0);

    ClusterNode node(gpu, cpu, ram, lan);
    Cluster cluster;
    cluster.AddNode(node);

    cluster.Print();
    cluster.Export("cluster_config.txt");
    return 0;
}



