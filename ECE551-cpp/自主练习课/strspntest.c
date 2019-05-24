#include <stdio.h>
#include <stdlib.h>

size_t strspn_m(const char * s1, const char * s2) {
	size_t i = 0;
	while (s2[i]!='\0'){
		i++;
	}
	size_t len_s2 = i;
	i = 0;
	while (i < len_s2) {
		if (s1[i]!=s2[i]){
			return i;
		}
		i++;
	}
	return i;
}

int strspn_p(const char *s,const char *accept)
{
    const char *p;
    const char *a;
    int count = 0;
    for(p = s; *p != '\0'; ++p){
        for (a = accept; *a != '\0'; ++a){
            if (*p == *a){
                ++count;
                break;
            }
        }
        if (*a == '\0'){
            return count;
        }
    }
    return count;
}


int main() {
	size_t ans = 0;
	int ans2 = 0;
	ans = strspn_m("abc", "1a2b3c");
	ans2 = strspn_p("abc", "1a2b3c");
	printf("%u,%d",ans,ans2);
}
