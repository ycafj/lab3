#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

const double ENGLISH_FREQ[] = {
	8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153,
	0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 6.327, 9.056,
	2.758, 0.978, 2.360, 0.150, 1.974, 0.074
};
struct Data {
	string text; //текст(біграми,триграми)
	int pos;// позиція в тексті
};
struct CharFreq {
	char character;
	double frequency;
};

vector<CharFreq> search_freq( string text);
string freq_analysis(string text,int prob_key); // частотний аналіз
string readfile(string filename);
string cleartext(string text); // очистка від розділових знаків, всі знаки в нижній регістр
string vizhenera(string text, string key); // шифр Віженера
string devizhenera(string text, string key); // дешифрування Віженера
vector<Data> find_bigr(string text); 
vector<vector<Data>> find_bigr2(vector<Data> bigrams);
vector<int> calc_key(vector<vector<Data>> bigrams);
vector<vector<int>> factorize(vector<int> n);
int main() {
	string text = cleartext(readfile("open.txt"));
	cout << text<<endl;
	string shifrtext = vizhenera(text, "crypto");
	cout << endl<<endl<<"Shifr text: " <<endl<< shifrtext << endl;
	vector<Data> bigrams = find_bigr(shifrtext);
	for(int i=0; i < bigrams.size(); i++) {
		cout << bigrams[i].text << " " << bigrams[i].pos << "  ";
	}
	cout << endl;
	vector<vector<int>> prob_len = factorize(calc_key(find_bigr2(bigrams)));
	cout << endl;
	
	int prob_key;
	cout << "Print possible key length" << endl;
	cin >> prob_key;
	cout<<devizhenera(shifrtext, freq_analysis(shifrtext, prob_key));
}



vector<CharFreq> search_freq(string text) {
	vector<CharFreq> frequencies(26);
	for(int i=0;i<26;i++) {
		frequencies[i].character = char(i + 'a');
		frequencies[i].frequency = 0.0;
		cout << frequencies[i].character << "  ";
	}
	int total_chars = 0;
	for (int i = 0; i < text.length(); i++) {
		for(int j=0;j<26;j++) {
			if(text[i]==frequencies[j].character) {
				frequencies[j].frequency++;
				total_chars++;
			}
		}
	}
	for(int i=0;i<26;i++) {
		frequencies[i].frequency = (frequencies[i].frequency / total_chars) * 100;
		cout << frequencies[i].frequency << "  ";
	}
	cout << total_chars << endl;
	return frequencies;
}
string freq_analysis(string text, int prob_key) {
	string clean_text = "";
	for (char c : text) {
		if (c != ' ') clean_text += c;
	}
	vector<string> groups(prob_key);
	for (int i = 0; i < clean_text.length(); i++) {
		groups[i % prob_key] += clean_text[i];
	}
	string found_key = "";
	for (int g = 0; g < prob_key; g++) {
		vector<CharFreq> current_freqs = search_freq(groups[g]);
		double min_difference = 1e18; 
		int best_shift = 0;
		for (int shift = 0; shift < 26; shift++) {
			double current_difference = 0.0;
			for (int i = 0; i < 26; i++) {
				int english_idx = (i - shift + 26) % 26;
				double observed = current_freqs[i].frequency;
				double expected = ENGLISH_FREQ[english_idx];
				current_difference += pow(observed - expected, 2);
			}
			if (current_difference < min_difference) {
				min_difference = current_difference;
				best_shift = shift;
			}
		}
		found_key += char(best_shift + 'a');
	}

	cout << "\n[RESULT] Probable key: " << found_key << endl;
	return found_key;
}
string readfile(string filename) {
	ifstream open(filename);
	string text, temp;
	while (getline(open, temp)) {
		text += temp;
	}
	open.close();
	return text;
}
string cleartext(string text) {
	string result = "";
	for (int i = 0; i < text.length(); i++) {
		if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z')) {
			result += tolower((unsigned char)text[i]); 
		}
		else if (text[i] == ' ') {
			result += ' ';
		}
	}
	return result;
}
string vizhenera(string text, string key) {
	string shifrtext;
	for(int i =0,j=0; i < text.length(); i++,j++) {
		if (text[i] == ' ') {
						shifrtext += ' ';
			j--;
			continue;
		}
		shifrtext += char((toascii(text[i]-97) + toascii(key[j % key.length()]-97)) % (toascii('z') - toascii('a')+1) + toascii('a'));
	}
	return shifrtext;
}
string devizhenera(string text, string key) {
	string shifrtext;
	for (int i = 0,j=0; i < text.length(); i++,j++) {
		if (text[i] == ' ') {
			shifrtext += ' ';
			j--;
			continue;
		}
		shifrtext += char((toascii(text[i]-97) - toascii(key[j % key.length()]-97)+ (toascii('z') - toascii('a')+1)) % (toascii('z') - toascii('a')+1) + toascii('a'));
	}
	return shifrtext;
}
vector<Data> find_bigr(string text) {
	vector<Data> bigrams;
	string temp;
	int pos=0,n=0,q=0;
	for (int i = 0; i < text.length(); i++) {
		if (text[i] == ' ') {
			if (n==2||n==3) {
				bigrams.push_back({ temp,pos-q });
			}
			q++;
			pos = i + 1;
			n = 0;
			temp = "";
			continue;
		}
		temp += text[i];
		n++;
	}
	return bigrams;
}
vector<vector<Data>> find_bigr2(vector<Data> bigrams) {
	vector<vector<Data>> bigrams2;
	int temp = 0;
	for (int i = 0; i < bigrams.size(); i++) {
		bool flag = false;
		for(int j=i+1;j<bigrams.size();) {
			if (bigrams[i].text == bigrams[j].text) {
				if(flag==false) {
					bigrams2.push_back(vector<Data>());
					bigrams2[temp].push_back({bigrams[i]});
					flag = true;
				}
				bigrams2[temp].push_back({bigrams[j]});
				bigrams.erase(bigrams.begin() + j);
			}
			else j++;
		}
		if (flag) temp++;

	}
	cout << endl;
	for(int i=0;i<bigrams2.size();i++) {
		for(int j=0;j<bigrams2[i].size();j++) {
			cout << bigrams2[i][j].text << " " << bigrams2[i][j].pos << "  ";
		}
	}
	cout << endl;
	return bigrams2;
}
vector<int> calc_key(vector<vector<Data>> bigrams) {
	vector<int> prob_len;
	int temp;

	for(int i=0;i<bigrams.size();i++) {
		for(int j=0;j<bigrams[i].size();j++) {
			for(int q=j+1;q<bigrams[i].size();q++) {
				if(j!=q) {
					temp = abs(bigrams[i][j].pos - bigrams[i][q].pos);
					prob_len.push_back(temp);
					cout << temp << "  ";
				}
			}
		}
	}
	return prob_len;
}
vector<vector<int>> factorize(vector<int> n) {
	vector<vector<int>> factors;
	for (int i = 0; i < n.size(); i++) {
		factors.push_back(vector<int>());
		while (n[i] % 2 == 0) {
			factors[i].push_back(2);
			n[i] /= 2;
		}
		for (long long j = 3; j <= sqrt(n[i]); j += 2) {
			while (n[i] % j == 0) {
				factors[i].push_back(j);
				n[i] /= j;
			}
		}
		if (n[i] > 1) {
			factors[i].push_back(n[i]);
		}
	}
	return factors;
}