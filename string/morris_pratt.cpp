#include <string>

using namespace std;

int pi[MAXSZ];

void kmp(string text, string pattern)
{
  pi[0] = -1;
  for(int i = 1; i < pattern.size(); i++)
  {
    pi[i] = pi[i-1];
    while(pi[i] >= 0 && pattern[pi[i] + 1] != pattern[i]) { pi[i] = pi[pi[i]]; }
    if(pattern[pi[i] + 1] == pattern[i]) pi[i]++;
  }

  int k = 0; //k + 1 é o tamanho do match atual
  for(int i = 0; i < text.size(); i++)
  {
    while(k >= 0 && pattern[k + 1] != text[i]) k = pi[k];
    if(pattern[k + 1] == text[i]) k++;
    if(k + 1 == pattern.size()) k = pi[k]; //achou!
  }
}
