#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1000;
const int MAXC = 8;
const int MAXR = 12;
const int MAXCOST = 20005;
const int MAXSTATES = 50000005;
const int CAP = MAXN+MAXC;

long long comb[MAXN+MAXC+2][MAXN+MAXC+2];
// m, l1 ... lC
int inSign[MAXC+3];
int outSign[MAXC+3];
int C, r;
int n;
int dp[MAXSTATES];
unsigned short qs[MAXSTATES];
map<int, int> previ;
int ansQ[MAXN];
set<string> buckets[MAXCOST];
vector<char> charCost[MAXC];
int sp[MAXN];
int d[MAXC];
int w[MAXR];
// dq[i, q]
int dq[MAXC][MAXN];

map<char32_t, int > charfreq;
vector<pair<int, char32_t>> charVec; 
int p[1000];

void init(long long maxi){
    for (long long i = 0; i <= maxi; i++){
        dp[i] = 0x7f7f7f7f;
    }
}

int encode_utf8(char32_t c, char out[4]) {
    if (c < 0x80) {
        out[0] = char(c);                 return 1;
    } else if (c < 0x800) {
        out[0] = char(0xC0 | (c >> 6));
        out[1] = char(0x80 | (c & 0x3F)); return 2;
    } else if (c < 0x10000) {
        out[0] = char(0xE0 | (c >> 12));
        out[1] = char(0x80 | ((c>>6)&0x3F));
        out[2] = char(0x80 | (c & 0x3F)); return 3;
    } else {
        out[0] = char(0xF0 | (c >> 18));
        out[1] = char(0x80 | ((c>>12)&0x3F));
        out[2] = char(0x80 | ((c>>6)&0x3F));
        out[3] = char(0x80 | (c & 0x3F)); return 4;
    }
}

void print32(char32_t c){
    char buf[4];
    int len = encode_utf8(c, buf);
    cout.write(buf, len);
}

void precompute_combs(){
    comb[0][0] = 1;
    for(int i = 1; i < CAP; i++){
        comb[i][0] = 1;
        for(int j = 1; j <= min(MAXC+1,i); j++){
            comb[i][j] = comb[i-1][j] + comb[i-1][j-1];
        }
    }
}

long long indexOf() {
    int t[C+1];
    t[C] = inSign[0];
    for (int i = C-1; i >= 0; i--){
        t[i] = t[i+1]+inSign[C-i]+1;
    }
    // t = m+l1+...+lC+C,m+l1+...+l(C-1)+C-1,... m+l1+1, m
    long long idx = 0;
    for (int i = 0; i <= C; i++){
        idx += comb[t[i]][C+1-i];
    }
    return idx + 1;
}

void signatureOf(long long ind) {
    long long idx = ind-1;
    // first recover t
    // t = m+l1+...+lC+C,m+l1+...+l(C-1)+C-1,... m+l1+1, m
    int t[C+1];
    int used = 0;
    for (int i = n+C; i >= 0; i--){
        if (idx >= comb[i][C+1-used]){
            idx -= comb[i][C+1-used];
            t[used] = i;
            used++;
            if (used > C) break;
        }
    }
    outSign[0] = t[C];
    for (int i = 1; i <= C; i++){
        outSign[i] = t[C-i]-t[C-i+1]-1;
    }
    return;
}

void reduce(int m){
    int mp = min(m, n);
    inSign[0] = mp;
    int ts = 0;
    for (int i = 1; i <= C; i++){
        inSign[i] = min(inSign[i],n-mp-ts);
        ts += inSign[i];
    }
}

int main(){
    auto t_start = chrono::high_resolution_clock::now();
    freopen("schmuck7.txt", "r", stdin);
    freopen("schmuck7.out", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    precompute_combs();

    cin >> r;
    for (int i = 1; i <= r; i++){
        cin >> w[i];
        d[w[i]]++;
        char c = char(i+int('0'));
        charCost[w[i]].push_back(c);
        string s(1, c);
        buckets[w[i]].insert(s);
        C = max(C, w[i]);
    } 

    string theString{istreambuf_iterator<char>(cin), {}};
    // first and last are \n, remove
    theString = theString.substr(1, theString.size() - 2);
    
    std::vector<char32_t> codepoints;
    for(size_t i = 0; i < theString.size();) {
        unsigned char lead = theString[i];
        size_t len;
        if      ((lead & 0x80) == 0x00) len = 1;
        else if ((lead & 0xE0) == 0xC0) len = 2;
        else if ((lead & 0xF0) == 0xE0) len = 3;
        else len = 4;

        char32_t cp = 0;
        if (len == 1) {
            cp = lead;
        } else if (len == 2) {
            cp  = (char32_t)(lead & 0x1F) << 6;
            cp |= (char32_t)(theString[i+1] & 0x3F);
        } else if (len == 3) {
            cp  = (char32_t)(lead & 0x0F) << 12;
            cp |= (char32_t)(theString[i+1] & 0x3F) <<  6;
            cp |= (char32_t)(theString[i+2] & 0x3F);
        } else {
            cp  = (char32_t)(lead & 0x07) << 18;
            cp |= (char32_t)(theString[i+1]&0x3F) << 12;
            cp |= (char32_t)(theString[i+2]&0x3F) <<  6;
            cp |= (char32_t)(theString[i+3]&0x3F);
        }
        codepoints.push_back(cp);
        i += len;
    }

    for (char32_t c : codepoints){
        auto it = charfreq.find(c);
        if (it == charfreq.end()){
            charfreq[c]=1;
        } else {
            charfreq[c]++;
        }
    }

    for (const auto& entry : charfreq) {
        p[++n] = entry.second;
        charVec.push_back({entry.second, entry.first});
    }

    int extra = (1 - n) % (r - 1);
    if (extra < 0) extra += (r - 1);
    for (int i = 1; i <= extra; i++){
        p[++n]=0;
        charfreq[n]=0;
        charVec.push_back({0,0});
    }
    sort(charVec.begin(), charVec.end());
    sort(p+1, p+n+1, greater<int>());

    for (int i = 1; i <= C; i++){
        for (int q = 0; q <= n; q++){
            dq[i][q] = q*d[i];
        }
    }
    for (int i = 1; i <= n; i++){
        //cout << p[i] << ' ';
        sp[i] = sp[i-1]+p[i];
    }
    //cout << '\n' << n <<' '<< C << ' ' << r << endl;

    outSign[C+1] = 0;
    // initialize first dp value:
    inSign[0] = 0;
    for (int i = 1; i <= C; i++){
        inSign[i] = d[i];
    }
    reduce(0);
    int initialIdx = indexOf();
    //cout << initialIdx << endl;

    long long total = comb[n+C+1][C+1];
    init(total);
    dp[initialIdx] = 0;
    //cout << total << endl;
    for (long long ind=1; ind<=total; ind++){
        if (dp[ind] == 0x7f7f7f7f) continue;
        signatureOf(ind);
        int m = outSign[0];
        int l1 = outSign[1];
        int newCost = dp[ind]-sp[m]+sp[n];

        for (int q = 0; q <= l1; q++){
            inSign[0] = m + l1 - q;
            int s = inSign[0];
            for (int i = 1; i <= C; i++){
                inSign[i] = outSign[i+1]+dq[i][q];
                s += inSign[i];
            }
            // reduction
            if (s > n) reduce(inSign[0]);

            long long newInd = indexOf();
            if (newCost < dp[newInd]){
                dp[newInd] = newCost;
                qs[newInd] = q;
                previ[newInd] = ind;
            }
        }
    }
    
    cout << dp[total] << endl;

    int j = 0;
    int curr = total;
    while (curr != initialIdx){
        ansQ[++j] = qs[curr];
        curr = previ[curr];
    }

    // j to 1, ascending order of cost
    for (int i = 1; i <= j; i++){
        int q = ansQ[j+1-i];
        for (int j = 1; j <= q; j++){
            string s = *buckets[i].begin();
            buckets[i].erase(s);
            for (int l = 1; l <= C; l++){
                for (char c: charCost[l]){
                    buckets[i+l].insert(s+c);
                }
            }
        }
    }

    int count = 0;
    for (int i = 1; i <= j+C; i++){
        for (string s: buckets[i]){
            print32(charVec[n-1-count].second);
            cout << ' ' << s << '\n';
            count++;
            if (count >= n-extra) break;
        }
        if (count >= n-extra) break;
    }


    auto t_end = chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<std::chrono::milliseconds>(t_end - t_start);

    cout << "Elapsed time: " 
              << elapsed.count() 
              << " ms\n";
    return 0;
}
