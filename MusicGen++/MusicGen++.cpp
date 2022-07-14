// MusicGen++.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms. //

#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include <map>

const std::string VERSION = "1.0";
const int SAMPLE_RATE = 44100;
const int DURATION = 1;
const int BIT_DEPTH = 16;
const int MAX_AMP = std::pow(2, BIT_DEPTH - 1) - 1;
const float PI = 3.1415926535897932;

// Command-Data
const std::vector<std::string> COMMAND_NAMES = {"note", "freq", "pause", "set_duration", "set_volume", "set_fadeout", "set_wave", "#"};
const std::map<std::string, float> NOTES = {
{"c0",16.35},
{"c#0",17.32},
{"db0",17.32},
{"d0",18.35},
{"d#0",19.45},
{"eb0",19.45},
{"e0",20.60},
{"f0",21.83},
{"f#0",23.12},
{"gb0",23.12},
{"g0",24.50},
{"g#0",25.96},
{"ab0",25.96},
{"a0",27.50},
{"a#0",29.14},
{"bb0",29.14},
{"b0",30.87},
{"c1",32.70},
{"c#1",34.65},
{"db1",34.65},
{"d1",36.71},
{"d#1",38.89},
{"eb1",38.89},
{"e1",41.20},
{"f1",43.65},
{"f#1",46.25},
{"gb1",46.25},
{"g1",49.00},
{"g#1",51.91},
{"ab1",51.91},
{"a1",55.00},
{"a#1",58.27},
{"bb1",58.27},
{"b1",61.74},
{"c2",65.41},
{"c#2",69.30},
{"db2",69.30},
{"d2",73.42},
{"d#2",77.78},
{"eb2",77.78},
{"e2",82.41},
{"f2",87.31},
{"f#2",92.50},
{"gb2",92.50},
{"g2",98.00},
{"g#2",103.83},
{"ab2",103.83},
{"a2",110.00},
{"a#2",116.54},
{"bb2",116.54},
{"b2",123.47},
{"c3",130.81},
{"c#3",138.59},
{"db3",138.59},
{"d3",146.83},
{"d#3",155.56},
{"eb3",155.56},
{"e3",164.81},
{"f3",174.61},
{"f#3",185.00},
{"gb3",185.00},
{"g3",196.00},
{"g#3",207.65},
{"ab3",207.65},
{"a3",220.00},
{"a#3",233.08},
{"bb3",233.08},
{"b3",246.94},
{"c4",261.63},
{"c#4",277.18},
{"db4",277.18},
{"d4",293.66},
{"d#4",311.13},
{"eb4",311.13},
{"e4",329.63},
{"f4",349.23},
{"f#4",369.99},
{"gb4",369.99},
{"g4",392.00},
{"g#4",415.30},
{"ab4",415.30},
{"a4",440.00},
{"a#4",466.16},
{"bb4",466.16},
{"b4",493.88},
{"c5",523.25},
{"c#5",554.37},
{"db5",554.37},
{"d5",587.33},
{"d#5",622.25},
{"eb5",622.25},
{"e5",659.25},
{"f5",698.46},
{"f#5",739.99},
{"gb5",739.99},
{"g5",783.99},
{"g#5",830.61},
{"ab5",830.61},
{"a5",880.00},
{"a#5",932.33},
{"bb5",932.33},
{"b5",987.77},
{"c6",1046.50},
{"c#6",1108.73},
{"db6",1174.66},
{"eb6",1244.51},
{"e6",1318.51},
{"f6",1396.91},
{"f#6",1479.98},
{"gb6",1479.98},
{"g6",1567.98},
{"g#6",1661.22},
{"ab6",1661.22},
{"a6",1760.00},
{"a#6",1864.66},
{"bb6",1864.66},
{"b6",1975.53},
{"c7",2093.00},
{"c#7",2217.46},
{"db7",2217.46},
{"d7",2349.32},
{"d#7",2489.02},
{"eb7",2489.02},
{"e7",2637.02},
{"f7",2793.83},
{"f#7",2959.96},
{"gb7",2959.96},
{"g7",3135.96},
{"g#7",3322.44},
{"ab7",3322.44},
{"a7",3520.00},
{"a#7",3729.31},
{"bb7",3729.31},
{"b7",3951.07},
{"c8",4186.01},
{"c#8",4434.92},
{"db8",4434.92},
{"d8",4698.63},
{"d#8",4978.03},
{"eb8",4978.03},
{"e8",5274.04},
{"f8",5587.65},
{"f#8",5919.91},
{"gb8",5919.91},
{"g8",6271.93},
{"g#8",6644.88},
{"ab8",6644.88},
{"a8",7040.00},
{"a#8",7458.62},
{"bb8",7458.62},
{"b8",7902.13}
};
enum COMMANDS {NOTE, FREQ, PAUSE, SET_DURATION, SET_VOLUME, SET_FADEOUT, SET_WAVE, COMMENT, EMPTY_LINE};

// Default Values
const float DEFAULT_VOLUME = 0.8;
const float DEFAULT_DURATION = 0.2;

class SoundWave {
  protected:
    float amp;
    float angle;
    float freq;
    float offset;

  public:
    SoundWave(float freq, float amp) : amp(amp) {
        angle = 0;
        set_freq(freq);
    }

    void set_freq(int freq) {
        this->freq = freq;
        offset = 2 * PI * freq / SAMPLE_RATE;
        angle += offset;
    }

    float calculate(bool square = false) {
        float sample = amp * sin(angle);
        if (square) sample = sample >= 0.0 ? amp : -amp;
        angle += offset;
        return sample;
    }

    float get_amp() {
        return amp;
    }

    void set_amp(float new_amp) {
        amp = new_amp;
    }

};

void write_fmt_bytes(std::ofstream &file, int value, int size) {
    file.write(reinterpret_cast<const char*> (&value), size);
}

int process_sample(float sample, int max_amplitude) {
    return static_cast<int> (sample * max_amplitude);
}

void generate_freq(std::ofstream &file, SoundWave &wave, float freq, float duration, bool fade_out = false, bool use_square_wave = true) {
    wave.set_freq(freq);

    float fade_factor = 0;
    float initial_amp = wave.get_amp();

    if (fade_out) {
        fade_factor = wave.get_amp() / (SAMPLE_RATE * duration);
    }

    for (int i = 0; i < SAMPLE_RATE * duration; i++) {
        int sample = process_sample(wave.calculate(use_square_wave), MAX_AMP);
        write_fmt_bytes(file, sample, 2);
        wave.set_amp(wave.get_amp() - fade_factor);
    }

    wave.set_amp(initial_amp);
}

void generate_silence(std::ofstream& file, float duration) {
    for (int i = 0; i < SAMPLE_RATE * duration; i++) {
        int sample = process_sample(0, MAX_AMP);
        write_fmt_bytes(file, sample, 2);
    }
}

void add_header(std::ofstream & file) {
    file << "RIFF----WAVEfmt ";
    write_fmt_bytes(file, 16, 4); /* Size of Meta-Data */
    write_fmt_bytes(file, 1, 2);  /* Compression-Type : 1 = PCM */
    write_fmt_bytes(file, 1, 2);  /* Amount of Audio-Channels */
    write_fmt_bytes(file, SAMPLE_RATE, 4); /* Samplerate */
    write_fmt_bytes(file, SAMPLE_RATE * BIT_DEPTH / 8, 4); /* Bytrate */
    write_fmt_bytes(file, (BIT_DEPTH / 8), 2); /* Block Align*/
    write_fmt_bytes(file, BIT_DEPTH, 2); /* Bitdepth */
    file << "data----";
}

std::vector<std::string> read_file(std::string file_name, bool verbose = false) {
    std::ifstream file(file_name);
    std::vector<std::string> lines;
    std::string current_line;

    while (std::getline(file, current_line)) {
        lines.push_back(current_line);
    }

    file.close();
    if (verbose) { std::cout << "[DEBUG] Read file with " << lines.size() << " lines" << std::endl; };
    return lines;
}

// Find and Replace (FReplace)
std::string freplace(const std::string text, std::string find_ss, std::string replace_ss) {
    int index = text.find(find_ss);
    if (index == -1) { return text; }

    return text.substr(0, index) + replace_ss + text.substr(index + find_ss.size(), text.size() - index - find_ss.size());
}

int main(int argc, char** argv)
{
    std::string input_file = "";
    std::string output_file = "";
    bool verbose = false;
    // Parsing args
    for (int i = 0; i < argc; ++i) {
        std::string current_arg = argv[i];
        if (current_arg == "-i") {
            i++;
            input_file = argv[i];
        }
        else if (current_arg == "-o") {
            i++;
            output_file = argv[i];
        }
        else if (current_arg == "-v" || current_arg == "--verbose") {
            verbose = true;
        }
        else if (current_arg == "--help" || current_arg == "-h") {
            std::cout << "--help / -h    | shows this help menu\n-i <filename>  | specifies input file\n-o <filename>  | speficies output file\n--verbose / -v | enables more debug output\n--version      | shows version info and copyright" << std::endl;
            exit(0);
        }
        else if (current_arg == "--version") {
            std::cout << "Paul's Music Generator v" << VERSION << "\nCopyright (c) 2022 The_Legend_of_xD" << std::endl;
            exit(0);
        }
    }

    if (input_file == "") {
        std::cerr << "[ERROR] Missing Input file" << std::endl;
        exit(2);
    } else if (output_file == "") {
        std::cerr << "[ERROR] Missing Output file" << std::endl;
        exit(3);
    }

    SoundWave sine_wave(233.08, DEFAULT_VOLUME);
    
    // Doing File stuff
    std::ofstream file;
    std::vector<std::string> lines = read_file(input_file, verbose);
    file.open(output_file, std::ios::binary); /* Kinder... Denkt immer 'dran, den Binary-Mode zu verwenden, wenn ihr Binaryfiles schreiben wollt! Nicht std::ios::out*/

    try {
        add_header(file);
        int pre_audio_pos = file.tellp();

        float current_volume = DEFAULT_VOLUME;
        float current_duration = DEFAULT_DURATION;
        bool fadeout_enabled = true;
        bool use_square_wave = true;

        // File Interpreting
        for (int i = 0; i < lines.size(); i++)
        {
            int current_command = -1;
            std::string params = "";

            // Extracting Commandtype and Params
            for (int x = 0; x < COMMAND_NAMES.size(); x++) {
                params = freplace(lines[i], COMMAND_NAMES[x], "");
                if (!(lines[i] == params)) { current_command = x; break; }
            }
            params = freplace(params, " ", "");
            if (verbose) { std::cout << "[DEBUG] \"" << lines[i] << "\" - \"" << params << "\" - Command-ID: " << current_command << '\n'; }

            // Command Execution
            switch (current_command) {
            case NOTE:
                generate_freq(file, sine_wave, NOTES.at(params), current_duration, fadeout_enabled, use_square_wave);
                break;
            case FREQ:
                generate_freq(file, sine_wave, std::stof(params), current_duration);
                break;
            case PAUSE:
                generate_silence(file, current_duration);
                break;
            case SET_VOLUME:
                current_volume = std::stof(params);
                sine_wave.set_amp(current_volume);
                break;
            case SET_DURATION:
                current_duration = std::stof(params);
                break;
            case SET_FADEOUT:
                if (params == "") {
                    fadeout_enabled = !fadeout_enabled;
                } else if (params == "true" || params == "on") {
                    fadeout_enabled = true;
                } else if (params == "false" || params == "off") {
                    fadeout_enabled = false;
                } else {
                    std::cout << "[WARNING] Invalid argument: for " << COMMAND_NAMES[current_command] << " : \"" << params << "\" (line:" << i + 1 << ")" << std::endl;
                }
                break;
            case SET_WAVE:
                if (params == "sine") {
                    use_square_wave = false;
                } else if (params == "square") {
                    use_square_wave = true;
                } else {
                    std::cout << "[WARNING] Invalid argument: for " << COMMAND_NAMES[current_command] << " : \"" << params << "\" (line:" << i + 1 << ")" << std::endl;
                }
                break;
            case COMMENT:
                break;
            default:
                if (lines[i] == "") { break; } // Prevent Empty lines of triggering a warning
                std::cout << "[WARNING] Invalid command: \"" << lines[i] << "\" (" << current_command << "; line:" << i+1 << ")" << std::endl;
                break;
            }
            
        }

        // Adding Chunnk sizes to Chunk-Headers
        int after_audio_pos = file.tellp();
        file.seekp((std::streampos)(pre_audio_pos - 4));
        write_fmt_bytes(file, after_audio_pos - pre_audio_pos, 4);
        file.seekp(std::ios_base::beg + 4);
        write_fmt_bytes(file, after_audio_pos - 8, 4);
    }
    catch (std::exception e) {
        std::cerr << e.what() << std::endl;
        file.close();
        exit(1);
    }
    file.close();
}