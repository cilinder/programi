#include <stdio.h>
#include <stdlib.h>

#define debug(); fprintf(stderr, "line: %d, stevec: %d\n", __LINE__, stevec);
#define count(); fprintf(stderr, "funkcija klicana: %d-krat\n", stevec);


int * mergesort(int a[], int length) {

    if ( length < 2 ) {
	return a;
    }
    int dolzina1, dolzina2;
    if ( length % 2 == 0 ) {
	dolzina1 = length/2;
	dolzina2 = length/2;
    }
    else {
	dolzina1 = length/2+1;
	dolzina2 = length/2;
    }

    int *b = malloc( dolzina1 * sizeof(int));
    int *c = malloc( dolzina2 * sizeof(int));
    for (int i=0; i<length; i++) {
	if ( i < dolzina1 )
	    b[i] = a[i];
	else {
	    c[i-dolzina1] = a[i];
	}
    }

    
    b = mergesort(b, dolzina1);
    c = mergesort(c, dolzina2);

    
    int *d = malloc(length * sizeof(int));
    if ( !d )
	return NULL;


    int i = 0;
    int j = 0;

    for (int k=0; k<length; k++) {
	if ( i < dolzina1 && j<dolzina2 ) {
	    if ( b[i] <= c[j] ) {
		d[k] = b[i];
		i++;
	    }
	    else if ( c[j] < b[i] ) {
		d[k] = c[j];
		j++;
	    }
	}
	else if ( i == dolzina1 ) {
	    d[k] = c[j];
	    j++;
	}
	else {
	    d[k] = b[i];
	    i++;
	}
    }

    free(b);
    free(c);
    return d;
    
}

int main() {
    
    int a[] = { 3, 2, 7, 1, 5, 6, 9, 8, 10, 3 };
    int *p;
    p = a;

    p = mergesort(a, 10);
    
    for(int i=0; i<10; i++) {
	printf("%d ", p[i]);
    }


}
