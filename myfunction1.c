#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "myfunction1.h"
#include "showBMP.h"
static char kernelscaleglobal;
int r1,r2,g1,g2,b1,b2;
static pixel applyKernel17(int i,int j, register pixel* src,bool first) {
    register pixel_sum sum;
    register pixel* iter;
    if(first){
        iter = src + i*n-n+j-1;
        sum.red=  ((int)(*iter).red);
        sum.green=((int)(*iter).green);
        sum.blue= ((int)(*iter).blue);

        r1=((int)  (*(iter+1)).red);
        g1=((int)(*(iter+1)).green);
        b1=((int) (*(iter+1)).blue);

        r2= ((int)  (*(iter+2)).red);
        g2=((int)(*(iter+2)).green);
        b2=((int) (*(iter+2)).blue);

        iter+=n;
        sum.red+=  ((int)(*iter).red);
        sum.green+=((int)(*iter).green);
        sum.blue+= ((int)(*iter).blue);

        r1+=((int)  (*(iter+1)).red);
        g1+=((int)(*(iter+1)).green);
        b1+=((int) (*(iter+1)).blue);

        r2+= ((int)  (*(iter+2)).red);
        g2+= ((int)(*(iter+2)).green);
        b2+= ((int) (*(iter+2)).blue);

        iter+=n;
        sum.red+=((int)  (*iter).red);
        sum.green+=((int)(*iter).green);
        sum.blue+=((int) (*iter).blue);

        r1+=((int)  (*(iter+1)).red);
        g1+=((int)(*(iter+1)).green);
        b1+=((int) (*(iter+1)).blue);

        r2+=((int)  (*(iter+2)).red);
        g2+=((int)(*(iter+2)).green);
        b2+=((int) (*(iter+2)).blue);
        sum.red += r1+r2;
        sum.blue += b1+b2;
        sum.green+=g1+g2;
    } else {
        //1 always before 2
        sum.red = r1 + r2;
        sum.blue = b1+b2;
        sum.green = g1+g2;
        iter = src + i*n-n+j-1;
        r1 = ((int)  (*(iter+2)).red);
        g1 = ((int)(*(iter+2)).green);
        b1 = ((int) (*(iter+2)).blue);
        iter+=n;
        r1+= ((int)  (*(iter+2)).red);
        g1+= ((int)(*(iter+2)).green);
        b1+= ((int) (*(iter+2)).blue);
        iter+=n;
        r1+=((int)  (*(iter+2)).red);
        g1+=((int)(*(iter+2)).green);
        b1+=((int) (*(iter+2)).blue);
        sum.red += r1;
        sum.green += g1;
        sum.blue += b1;
        int tmp = r1;
        r1 = r2;
        r2 = tmp;
        tmp = g1;
        g1 = g2;
        g2 = tmp;
        tmp=b1;
        b1 = b2;
        b2 = tmp;
    }

    register int current, min_intensity,max_intensity , min_row, min_col, max_row, max_col; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765

    // find min and max coordinates

    //i-1, j-1
    // check if smaller than min or higher than max and update
    iter -= (n<<1);
    current = (((int) (*iter).red) + ((int) (*iter).green) + ((int) (*iter).blue));
    max_intensity = current;
    min_intensity = current;
    max_row = i-1;
    min_row = i-1;
    min_col = j-1;
    max_col = j-1;

    //i-1, j

    // check if smaller than min or higher than max and update
    current = (((int) (*(iter+1)).red) + ((int) (*(iter+1)).green) + ((int) (*(iter+1)).blue));
    if (current <= min_intensity) {
        min_intensity = current;
        min_row = i-1;
        min_col = j;
    }else if (current > max_intensity) {
        max_intensity = current;
        max_row = i-1;
        max_col = j;
    }

    //i-1, j+1
    // check if smaller than min or higher than max and update
    current = (((int) (*(iter+2)).red) + ((int) (*(iter+2)).green) + ((int) (*(iter+2)).blue));
    if (current <= min_intensity) {
        min_intensity = current;
        min_row = i-1;
        min_col = j+1;
    }else if (current > max_intensity) {
        max_intensity = current;
        max_row = i-1;
        max_col = j+1;
    }

    //i, j-1
    // check if smaller than min or higher than max and update
    iter+=n;
    current = (((int) (*iter).red) + ((int) (*iter).green) + ((int) (*iter).blue));
    if (current <= min_intensity) {
        min_intensity = current;
        min_row = i;
        min_col = j-1;
    }else if (current > max_intensity) {
        max_intensity = current;
        max_row = i;
        max_col = j-1;
    }

    //i, j
    // check if smaller than min or higher than max and update
    current = (((int) (*(iter+1)).red) + ((int) (*(iter+1)).green) + ((int) (*(iter+1)).blue));
    if (current <= min_intensity) {
        min_intensity = current;
        min_row = i;
        min_col = j;
    }else if (current > max_intensity) {
        max_intensity = current;
        max_row = i;
        max_col = j;
    }


    //i, j+1
    // check if smaller than min or higher than max and update
    current = (((int) (*(iter+2)).red) + ((int) (*(iter+2)).green) + ((int) (*(iter+2)).blue));
    if (current <= min_intensity) {
        min_intensity = current;
        min_row = i;
        min_col = j+1;
    }else if (current > max_intensity) {
        max_intensity = current;
        max_row = i;
        max_col = j+1;
    }
    //i+1, j-1
    // check if smaller than min or higher than max and update
    iter+=n;
    current = (((int) (*iter).red) + ((int) (*iter).green) + ((int) (*iter).blue));
    if (current <= min_intensity) {
        min_intensity = current;
        min_row = i+1;
        min_col = j-1;
    }else if (current > max_intensity) {
        max_intensity = current;
        max_row = i+1;
        max_col = j-1;
    }

    //i+1, j
    // check if smaller than min or higher than max and update
    current = (((int) (*(iter+1)).red) + ((int) (*(iter+1)).green) + ((int) (*(iter+1)).blue));
    if (current <= min_intensity) {
        min_intensity = current;
        min_row = i+1;
        min_col = j;
    }else if (current > max_intensity) {
        max_intensity = current;
        max_row = i+1;
        max_col = j;
    }

    //i+1, j+1
    // check if smaller than min or higher than max and update
    current = (((int) (*(iter+2)).red) + ((int) (*(iter+2)).green) + ((int) (*(iter+2)).blue));
    if (current <= min_intensity) {
        min_intensity = current;
        min_row = i+1;
        min_col = j+1;
    }else if (current > max_intensity) {
        max_intensity = current;
        max_row = i+1;
        max_col = j+1;
    }

    // filter out min and max      max_row*dim+max_col
    pixel loop_pixel = src[min_row*n+min_col];
    sum.red -= ((int) loop_pixel.red);
    sum.green -= ((int) loop_pixel.green);
    sum.blue -= ((int) loop_pixel.blue);
    loop_pixel = src[max_row*n+max_col];
    sum.red -= ((int) loop_pixel.red);
    sum.green -= ((int) loop_pixel.green);
    sum.blue -= ((int) loop_pixel.blue);

    register pixel current_pixel;
    // assign kernel's result to pixel at [i,j]
    sum.red  /= 7;
    sum.green  /= 7;
    sum.blue /= 7;
    current_pixel.red = (unsigned char) (sum.red < 0 ? 0 : (sum.red > 255 ? 255 : sum.red));
    current_pixel.green = (unsigned char) (sum.green < 0 ? 0 : (sum.green > 255 ? 255 : sum.green));
    current_pixel.blue = (unsigned char) (sum.blue < 0 ? 0 : (sum.blue > 255 ? 255 : sum.blue));

    return current_pixel;
}
static pixel applyKernel19(int i,int j, register pixel* src,bool first) {
    register pixel_sum sum;
    register pixel* iter;
    if(first){
        iter = src + (i-1)*n+j-1;
        sum.red=  ((int)(*iter).red);
        sum.green=((int)(*iter).green);
        sum.blue= ((int)(*iter).blue);

        r1=((int)  (*(iter+1)).red);
        g1=((int)(*(iter+1)).green);
        b1=((int) (*(iter+1)).blue);

        r2= ((int)  (*(iter+2)).red);
        g2=((int)(*(iter+2)).green);
        b2=((int) (*(iter+2)).blue);


        iter+=n;
        sum.red+=  ((int)(*iter).red);
        sum.green+=((int)(*iter).green);
        sum.blue+= ((int)(*iter).blue);

        r1+=((int)  (*(iter+1)).red);
        g1+=((int)(*(iter+1)).green);
        b1+=((int) (*(iter+1)).blue);

        r2+= ((int)  (*(iter+2)).red);
        g2+= ((int)(*(iter+2)).green);
        b2+= ((int) (*(iter+2)).blue);

        iter+=n;
        sum.red+=((int)  (*iter).red);
        sum.green+=((int)(*iter).green);
        sum.blue+=((int) (*iter).blue);

        r1+=((int)  (*(iter+1)).red);
        g1+=((int)(*(iter+1)).green);
        b1+=((int) (*(iter+1)).blue);

        r2+=((int)  (*(iter+2)).red);
        g2+=((int)(*(iter+2)).green);
        b2+=((int) (*(iter+2)).blue);
        sum.red += r1+r2;
        sum.blue += b1+b2;
        sum.green+=g1+g2;
    } else {
        //1 always before 2
        sum.red = r1 + r2;
        sum.blue = b1+b2;
        sum.green = g1+g2;

        iter = src + (i-1)*n+j-1;
        r1 = ((int)  (*(iter+2)).red);
        g1 = ((int)(*(iter+2)).green);
        b1 = ((int) (*(iter+2)).blue);
        iter+=n;
        r1+= ((int)  (*(iter+2)).red);
        g1+= ((int)(*(iter+2)).green);
        b1+= ((int) (*(iter+2)).blue);
        iter+=n;
        r1+=((int)  (*(iter+2)).red);
        g1+=((int)(*(iter+2)).green);
        b1+=((int) (*(iter+2)).blue);
        sum.blue += b1;
        sum.green += g1;
        sum.red += r1;
        register int tmp = r1;
        r1 = r2;
        r2 = tmp;
        tmp = g1;
        g1 = g2;
        g2 = tmp;
        tmp=b1;
        b1 = b2;
        b2 = tmp;
    }

    register pixel current_pixel;
    // assign kernel's result to pixel at [i,j]
    sum.red   /=9  ;
    sum.green /= 9;
    sum.blue  /= 9;
    current_pixel.red = (unsigned char) (sum.red < 0 ? 0 : (sum.red > 255 ? 255 : sum.red));
    current_pixel.green = (unsigned char) (sum.green < 0 ? 0 : (sum.green > 255 ? 255 : sum.green));
    current_pixel.blue = (unsigned char) (sum.blue < 0 ? 0 : (sum.blue > 255 ? 255 : sum.blue));

    return current_pixel;
}
static pixel applyKernel2(int i,int j, register pixel* src) {
    register pixel_sum sum;
    register pixel* iter = src + i*n-n+j-1;
    sum.red=-((int)  (*iter).red);
    sum.green=-((int)(*iter).green);
    sum.blue=-((int) (*iter).blue);
    sum.red-=((int)  (*(iter+1)).red);
    sum.green-=((int)(*(iter+1)).green);
    sum.blue-=((int) (*(iter+1)).blue);

    sum.red-=((int)  (*(iter+2)).red);
    sum.green-=((int)(*(iter+2)).green);
    sum.blue-=((int) (*(iter+2)).blue);
    iter+=n;
    sum.red-=((int)  (*iter).red);
    sum.green-=((int)(*iter).green);
    sum.blue-=((int) (*iter).blue);


    sum.red+=((int)  (*(iter+1)).red)*9;
    sum.green+=((int)(*(iter+1)).green)*9;
    sum.blue+=((int) (*(iter+1)).blue)*9;

    sum.red-=((int)  (*(iter+2)).red);
    sum.green-=((int)(*(iter+2)).green);
    sum.blue-=((int) (*(iter+2)).blue);

    iter+=n;
    sum.red-=((int)  (*iter).red);
    sum.green-=((int)(*iter).green);
    sum.blue-=((int) (*iter).blue);

    sum.red-=((int)  (*(iter+1)).red);
    sum.green-=((int)(*(iter+1)).green);
    sum.blue-=((int) (*(iter+1)).blue);

    sum.red-=((int)  (*(iter+2)).red);
    sum.green-=((int)(*(iter+2)).green);
    sum.blue-=((int) (*(iter+2)).blue);
    register pixel current_pixel;
    // assign kernel's result to pixel at [i,j]
    current_pixel.red = (unsigned char) (sum.red < 0 ? 0 : (sum.red > 255 ? 255 : sum.red));
    current_pixel.green = (unsigned char) (sum.green < 0 ? 0 : (sum.green > 255 ? 255 : sum.green));
    current_pixel.blue = (unsigned char) (sum.blue < 0 ? 0 : (sum.blue > 255 ? 255 : sum.blue));

    return current_pixel;
}
static pixel applyKernel3(int i,int j, register pixel* src) {

    register pixel_sum sum;

    register pixel* iter = src + i*n+j-1;

    sum.red=((int)  (*iter).red);
    sum.green=((int)(*iter).green);
    sum.blue=((int) (*iter).blue);

    sum.red+=((int)  (*(iter+1)).red)<<1;
    sum.green+=((int)(*(iter+1)).green)<<1;
    sum.blue+=((int) (*(iter+1)).blue)<<1;

    sum.red+=((int)  (*(iter+2)).red);
    sum.green+=((int)(*(iter+2)).green);
    sum.blue+=((int) (*(iter+2)).blue);
    register pixel current_pixel;
    // assign kernel's result to pixel at [i,j]
    sum.red >>=2;
    sum.green>>=2;
    sum.blue >>=2;
    current_pixel.red = (unsigned char) (sum.red < 0 ? 0 : (sum.red > 255 ? 255 : sum.red));
    current_pixel.green = (unsigned char) (sum.green < 0 ? 0 : (sum.green > 255 ? 255 : sum.green));
    current_pixel.blue = (unsigned char) (sum.blue < 0 ? 0 : (sum.blue > 255 ? 255 : sum.blue));

    return current_pixel;
}
static pixel applyKernel4(int i,int j, register pixel* src) {

    register pixel_sum sum;

    register pixel* iter = src + i*n+j-1;
    sum.red=-(((int)  (*iter).red)<<1);
    sum.green=-(((int)(*iter).green)<<1);
    sum.blue=-(((int) (*iter).blue)<<1);

    sum.red+=(((int)  (*(iter+1)).red)*3)<<1;
    sum.green+=(((int)(*(iter+1)).green)*3)<<1;
    sum.blue+=(((int) (*(iter+1)).blue)*3)<<1;

    sum.red+=-(((int)  (*(iter+2)).red)<<1);
    sum.green+=-(((int)(*(iter+2)).green)<<1);
    sum.blue+=-(((int) (*(iter+2)).blue)<<1);
    register pixel current_pixel;
    // assign kernel's result to pixel at [i,j]
    sum.red >>= 1;
    sum.green >>=1;
    sum.blue >>=1;
    current_pixel.red = (unsigned char) (sum.red < 0 ? 0 : (sum.red > 255 ? 255 : sum.red));
    current_pixel.green = (unsigned char) (sum.green < 0 ? 0 : (sum.green > 255 ? 255 : sum.green));
    current_pixel.blue = (unsigned char) (sum.blue < 0 ? 0 : (sum.blue > 255 ? 255 : sum.blue));

    return current_pixel;
}
void smooth(register pixel *dst, char kernelindex, register pixel* src) {
    //pixel (*functionPointer)(int, int, int, pixel*);
    //kernel size is 3
    bool b;
    register int i, j, iDim = 0, idimj;
    switch (kernelindex) {
        case 1:
            if (kernelscaleglobal==7) {
                for (i = 1 ; i < n - 1; i++) {
                    iDim += n;
                    idimj = iDim;
                    b = true;
                    for (j =  1 ; j < n - 1 ; j++) {
                        dst[++idimj] = applyKernel17(i, j,src, b);
                        b = false;
                    }
                }
            } else{
                for (i = 1 ; i < n - 1; i++) {
                    iDim += n;
                    idimj = iDim;
                    b = true;
                    for (j =  1 ; j < n - 1 ; j++) {
                        dst[++idimj] = applyKernel19(i, j,src, b);
                        b = false;
                    }
                }
            }

            break;
        case 2:
            for (i = 1 ; i < n - 1; i++) {
                iDim += n;
                idimj = iDim;
                for (j =  1 ; j < n - 1 ; j+=2) {
                    dst[++idimj] = applyKernel2( i, j,src);
                    dst[++idimj] = applyKernel2( i, j+1,src);
                }
                for(;j<n-1;j++){
                    dst[++idimj] = applyKernel2( i, j,src);
                }
            }
            break;
        case 3:
            for (i = 1 ; i < n - 1; i++) {
                iDim += n;
                idimj = iDim;
                for (j =  1 ; j < n - 1 ; j+=2) {
                    dst[++idimj] = applyKernel3(i, j,src);
                    dst[++idimj] = applyKernel3(i, j+1,src);
                }
                for(;j<n-1;j++) {
                    dst[++idimj] = applyKernel3(i, j,src);
                }
            }
            break;
        default:
            for (i = 1 ; i < n - 1; i++) {
                iDim += n;
                idimj = iDim;
                for (j =  1 ; j < n - 1 ; j++) {
                    dst[++idimj] = applyKernel4(i, j,src);
                }
            }
            break;
    }
}
void charsToPixels(register Image *image, register pixel* pixels) {
    register pixel* iter1;
    register char* iter2;
    register int row, col, nrow = -n, nrowcol;
    for (row = 0 ; row < n ; row++) {
        nrow+=n;
        nrowcol=nrow-10;
        for (col = 0 ; col < n ; col+=10) {
            nrowcol+=10;
            iter1=pixels+nrowcol;
            iter2 = image->data+nrowcol*3;
            (*iter1).red = (*iter2++);
            (*iter1).green = (*iter2++);
            (*iter1).blue = (*iter2++);

            (*(++iter1)).red = (*iter2++);
            (*(iter1)).green = (*iter2++);
            (*(iter1)).blue = (*iter2++);

            (*(++iter1)).red = (*iter2++);
            (*(iter1)).green = (*iter2++);
            (*(iter1)).blue = (*iter2++);

            (*(++iter1)).red = (*iter2++);
            (*(iter1)).green =(*iter2++);
            (*(iter1)).blue =(*iter2++);

            (*(++iter1)).red = (*iter2++);
            (*(iter1)).green = (*iter2++);
            (*(iter1)).blue = (*iter2++);

            (*(++iter1)).red = (*iter2++);
            (*(iter1)).green = (*iter2++);
            (*(iter1)).blue = (*iter2++);

            (*(++iter1)).red = (*iter2++);
            (*(iter1)).green = (*iter2++);
            (*(iter1)).blue = (*iter2++);

            (*(++iter1)).red = (*iter2++);
            (*(iter1)).green = (*iter2++);
            (*(iter1)).blue = (*iter2++);

            (*(++iter1)).red = (*iter2++);
            (*(iter1)).green = (*iter2++);
            (*(iter1)).blue = (*iter2++);

            (*(++iter1)).red = (*iter2++);
            (*(iter1)).green = (*iter2++);
            (*(iter1)).blue = (*iter2++);
        }
        for (; col < n ; col++) {
            nrowcol++;
            pixels[nrowcol].red = image->data[3 * nrowcol];
            pixels[nrowcol].green = image->data[3 * nrowcol + 1];
            pixels[nrowcol].blue = image->data[3 * nrowcol + 2];
        }
    }
}
void doConvolution(Image *image, char kernelindex, char kernelScale) {
    kernelscaleglobal = kernelScale;
    pixel* pixelsImg = malloc(m*m*sizeof(pixel));
    charsToPixels(image, pixelsImg);
    smooth( pixelsImg, kernelindex,(pixel*)image->data);
    free(image->data);
    image->data = (char*)pixelsImg;

}