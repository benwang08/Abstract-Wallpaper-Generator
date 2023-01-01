#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>

using namespace std;

typedef pair<int, long> pi;
//rounds to nearest 10
inline int round(int in, int round){
    int base = in / round;
    base *= round;

    int half = round / 2;
    int temp = in - base;

    if (temp > half){
        return base + round;
    }
    else{
        return base;
    }
}

string pixel_to_str(int in){
    return to_string(in) + "p";
}

vector<int> str_to_pixels(string in){
    string temp = "";
    vector<int> rgb;
    for(int i = 0; i < in.size(); i++){
        if (in[i] == 'p'){
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
    int PIXEL_ROUND = 50;
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
        string r_string = pixel_to_str(round(r, PIXEL_ROUND));
        string g_string = pixel_to_str(round(g, PIXEL_ROUND));
        string b_string = pixel_to_str(round(b, PIXEL_ROUND));
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
    priority_queue<int, vector<int>, greater<int>> pq;
    unordered_map<int, vector<int>> pixel_counts;
    for (auto& it: pixels){
        if (pq.size() >= 30){
            int comp = pq.top();
            if (it.second > pq.top()){
                total -= pq.top();
                pixel_counts.erase(pq.top());
                pq.pop();
            }
            else{
                continue;
            }
        }
        total += it.second;
        pixel_counts[it.second] = str_to_pixels(it.first);
        pq.push(it.second);
    }
    priority_queue<int, vector<int>, greater<int>> copy = pq;

    ofstream img("color_palette.ppm");

    img << "P3\n" << 100 << ' ' << 1500 << "\n255\n";

    vector<int> color = pixel_counts[copy.top()];
    copy.pop();
    for (int j = 1499; j >= 0; --j) {
        if (j % 50 == 0){
            color = pixel_counts[copy.top()];
            copy.pop();
        }
        for (int i = 0; i < 100; i++) {
            img << color[0] << " ";
            img << color[1] << " ";
            img << color[2] << endl;
        }
    }

    img.close();

    ofstream of("data.txt");
    of << width << " " << height << endl;
    while(!pq.empty()){
        double percent = pq.top() / double(total);
        vector<int> color = pixel_counts[pq.top()];

        of << color[0] << " " << color[1] << " " << color[2] << " ";
        of << percent << endl;
        pq.pop();
    }

    of.close();


}
