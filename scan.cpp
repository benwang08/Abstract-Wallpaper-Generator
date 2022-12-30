#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>

using namespace std;

typedef pair<int, long> pi;
//rounds to nearest 10
inline int round(int in){
    string temp = to_string(in);
    char last_char = temp[temp.size() - 1];
    int last = last_char - '0';

    int base = in / 5;
    base *= 5;
    if (last == 5 || last == 0){
        return in;
    }
    if (last == 3 || last == 4 || last == 8 || last == 9){
        return base + 5;
    }
    return base;
}

inline int round_ten(int in){
    int base = in / 10;
    int rem = in - base;
    if (rem >= 5){
        return base + 10;
    }
    return base;
}

string pixel_to_str(int in){
    return to_string(in) + "1";
}

vector<int> str_to_pixels(string in){
    string temp = "";
    vector<int> rgb;
    for(int i = 0; i < in.size(); i++){
        if (in[i] == '1'){
            if (temp == ""){
                temp += in[i];
                continue;
            }
            rgb.push_back(stoi(temp));
            temp = "";
        }
        else{
            temp += in[i];
        }
    }

    return rgb;
}

int main(int argc, char *argv[]){
    //command line parsing and checking
    string temp_fname = argv[1];
    int width = stoi(argv[2]);
    int height = stoi(argv[3]);

    //convert file name to .ppm extension
    string file_name = "";
    for (int i = 0; i < temp_fname.size(); i++){
        if (temp_fname[i] == '.'){
            break;
        }
        file_name += temp_fname[i];
    }
    file_name += ".ppm";

    unordered_map<string, int> pixels;
    fstream fs(file_name);

    //convert lakers.jpg -compress none lakers.ppm
    //skip header lines
    string temp;
    for(int i = 0; i < 4; i++){
        getline(fs, temp);
    }

    int r, g, b;
    while(!fs.eof()){
        fs >> r >> g >> b;
        string r_string = pixel_to_str(round_ten(r));
        string g_string = pixel_to_str(round_ten(g));
        string b_string = pixel_to_str(round_ten(b));
        string hash_string = r_string + g_string + b_string;

        if (pixels.find(hash_string) == pixels.end()){
            pixels[hash_string] = 1;
        }
        else{
            pixels[hash_string]++;
        }
    }
    fs.close();

    int total = 0;
    priority_queue<pi, vector<pi>, greater<pi>> pq;

    for (auto& it: pixels){
        if (pq.size() >= 30){
            int comp = pq.top().first;
            if (it.second > pq.top().first){
                total -= pq.top().first;
                pq.pop();
            }
            else{
                continue;
            }
        }
        total += it.second;
        pair<int, long> temp(it.second, stol(it.first));
        pq.push(temp);
    }
    priority_queue<pi, vector<pi>, greater<pi>> copy = pq;

    std::cout << "P3\n" << 100 << ' ' << 1500 << "\n255\n";

    vector<int> color = str_to_pixels(to_string(copy.top().second));
    copy.pop();
    for (int j = 1499; j >= 0; --j) {
        if (j % 50 == 0){
            color = str_to_pixels(to_string(copy.top().second));
            copy.pop();
        }
        for (int i = 0; i < 100; i++) {
            cout << color[0] << " ";
            cout << color[1] << " ";
            cout << color[2] << endl;
        }
    }

    ofstream of("data.txt");
    while(!pq.empty()){
        double percent = pq.top().first / double(total);
        vector<int> color = str_to_pixels(to_string(pq.top().second));

        of << color[0] << " " << color[1] << " " << color[2] << " ";
        of << percent << endl;
        pq.pop();
    }

    of.close();


}
