bool k_cmp(int a1, int b1, int a2, int b2, int a3 = 0, int b3 = 0) {
    return a1 != b1 ? a1 < b1 : (a2 != b2 ? a2 < b2 : a3 < b3);
}

int bucket[MAXSZ+3], tmp[MAXSZ+3];
template<class T> void k_radix(T keys, int *in, int *out,
                               int off, int n, int k) {
    memset(bucket, 0, sizeof(int) * (k+1));

    for(int j = 0; j < n; j++)
        bucket[keys[in[j]+off]]++;
    for(int j = 0, sum = 0; j <= k; j++)
        sum += bucket[j], bucket[j] = sum - bucket[j];
    for(int j = 0; j < n; j++)
        out[bucket[keys[in[j]+off]]++] = in[j];
}

int mod0[MAXSZ/3+10];
vector<int> k_rec(const vector<int>& v, int k) {
    int n = v.size()-3, sz = (n+2)/3, sz2 = sz + n/3;
    if(n < 2) return vector<int>(n);

    vector<int> sub(sz2+3);
    for(int i = 1, j = 0; j < sz2; i += i%3, j++)
        sub[j] = i;

    k_radix(v.begin(), &sub[0], tmp, 2, sz2, k);
    k_radix(v.begin(), tmp, &sub[0], 1, sz2, k);
    k_radix(v.begin(), &sub[0], tmp, 0, sz2, k);

    int last[3] = {-1, -1, -1}, unique = 0;
    for(int i = 0; i < sz2; i++) {
        bool diff = false;
        for(int j = 0; j < 3; last[j] = v[tmp[i]+j], j++)
            diff |= last[j] != v[tmp[i]+j];
        unique += diff;

        if(tmp[i]%3 == 1) sub[tmp[i]/3] = unique;
        else sub[tmp[i]/3 + sz] = unique;
    }

    vector<int> rec;
    if(unique < sz2) {
        rec = k_rec(sub, unique);
        rec.resize(n);
        for(int i = 0; i < sz2; i++) sub[rec[i]] = i+1;
    } else {
        rec.resize(n);
        for(int i = 0; i < sz2; i++) rec[sub[i]-1] = i;
    }

    for(int i = 0, j = 0; j < sz; i++)
        if(rec[i] < sz)
            tmp[j++] = 3*rec[i];
    k_radix(v.begin(), tmp, mod0, 0, sz, k);
    for(int i = 0; i < sz2; i++)
        rec[i] = rec[i] < sz ? 3*rec[i] + 1 : 3*(rec[i] - sz) + 2;

    int prec = sz2-1, pmod0 = sz-1, pret = n-1, srec = n%3==1;
    while(prec >= srec && pmod0 >= 0)
        if(rec[prec]%3 == 1 && k_cmp(v[mod0[pmod0]], v[rec[prec]],
                                     sub[mod0[pmod0]/3], sub[rec[prec]/3+sz]) ||
           rec[prec]%3 == 2 && k_cmp(v[mod0[pmod0]], v[rec[prec]],
                                     v[mod0[pmod0]+1], v[rec[prec]+1],
                                     sub[mod0[pmod0]/3+sz], sub[rec[prec]/3+1]))
            rec[pret--] = rec[prec--];
        else
            rec[pret--] = mod0[pmod0--];
    if(pmod0 >= 0) memcpy(&rec[0], mod0, sizeof(int) * (pmod0+1));

    return rec;
}

vector<int> karkkainen(string s) {
    int n = s.size(), cnt = 1;
    vector<int> v(n + 3);

    for(int i = 0; i < n; i++) v[i] = i;
    k_radix(s.begin(), &v[0], tmp, 0, n, 256);
    for(int i = 0; i < n; cnt += (i+1 < n && s[tmp[i+1]] != s[tmp[i]]), i++)
        v[tmp[i]] = cnt;

    return k_rec(v, cnt);
}

vector<int> lcp(const string& s, const vector<int>& sa) {
    int n = sa.size();
    vector<int> prm(n), ans(n-1);
    for(int i = 0; i < n; i++) prm[sa[i]] = i;

    for(int h = 0, i = 0; i < n; i++)
        if(prm[i]) {
            int j = sa[prm[i]-1], ij = max(i, j);
            while(ij + h < (int)s.size() && s[i+h] == s[j+h]) h++;
            ans[prm[i]-1] = h;
            if(h) h--;
        }
    return ans;
}
