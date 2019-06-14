
#include "header.h"
#include "bmpFile.h"
#ifndef __BmpFile
#define __BmpFile
#endif
using namespace std;


int BmpFile::isFileExist(char *filename)
{
    FILE *bmpFilename;
    bmpFilename = fopen(filename,"rb");

    if(bmpFilename == NULL) {
        throw cipher_error ("File doesn't exits or wrong Filename.\n");
        return (-1);
    }

    fclose(bmpFilename);
    return 0;
}


int BmpFile::readHeader(char *filename)
{
    char m1,m2;
    unsigned long width, height, fileSize;

    if(isFileExist(filename) == 0) {
        FILE *bmpFilename = fopen(filename,"rb");

        fread((char *)&m1,1,1,bmpFilename);
        fread((char *)&m2,1,1,bmpFilename);

        if(m1 != 'B' || m2 != 'M') {
            throw cipher_error ("error: not a valid bitmap file");
            fclose(bmpFilename);
            return -1;
        }

        bmpIdentifier = m1 * 100 + m2;

        fread((long *)&bmpFilesize,4,1,bmpFilename);

        fread((unsigned short int *)&bmpres1,2,1,bmpFilename);

        fread((unsigned short int *)&bmpres2,2,1,bmpFilename);

        fread((long *)&bmpPixoff,4,1,bmpFilename);

        fread((long *)&bmpiSize,4,1,bmpFilename);

        fread((long *)&bmpWidth,4,1,bmpFilename);

        fread((long *)&bmpHeight,4,1,bmpFilename);

        fread((unsigned short int *)&bmpPlanes,2,1,bmpFilename);

        fread((unsigned short int *)&(bmpBitsPixel),2,1,bmpFilename);

        fread((long *)&bmpCompression,4,1,bmpFilename);

        fread((long *)&bmpImageSize,4,1,bmpFilename);

        fread((long *)&bmpXscale,4,1,bmpFilename);

        fread((long *)&bmpYScale,4,1,bmpFilename);

        fread((long *)&bmpColor,4,1,bmpFilename);

        fread((long *)&bmpImpCol,4,1,bmpFilename);

        fclose(bmpFilename);

        width = bmpWidth;
        height = bmpHeight;
        fileSize = width * height * 3;
        bmpTotalStuffablechar = (fileSize/8)-54;

        return 0;
    }
    return -1;
}


int BmpFile::getDimension(char *filename, long *width, long *height)
{
    if(isFileExist(filename) == 0) {
        readHeader(filename);
        *width = bmpWidth;
        *height = bmpHeight;
        return 0;
    }
    return -1;
}


BmpFile::BmpFile(char *filename)
{
    if(isFileExist(filename) == 0) {
        readHeader(filename);
    }
}

int BmpFile::checkFilesForHiding(char *bmpfile, char *txtfile)
{
    FILE *bfile, *tfile;
    long l = 0;


    bfile = fopen(bmpfile,"rb");

    if(isFileExist(bmpfile) == -1) {
        throw cipher_error ("BMP file doesn't exist");
        return -1;
    }

    tfile = fopen(txtfile,"rb");
    if(isFileExist(txtfile)== -1) {
        throw cipher_error ("txt file doesn't exist");
        return -1;
    }

    while( ! feof(tfile) ) {			// read the length of txt file...
        fgetc(tfile);
        l++;
    }

    readHeader(bmpfile);

    if(l >= bmpTotalStuffablechar) {
        throw cipher_error ("length of text file too big , select bigger bmp file");
        return -1;
    }

    fclose(bfile);
    fclose(tfile);
    return 0;
}

int BmpFile::hide(char *bmpfile, char *txtfile, char *output)
{

    FILE *bfile, *tfile, *outfile;
    unsigned char header[54];
    char bmpbuffer, txtbuffer;
    int i;
    char txtTerminatorIndicator = '*';

    checkFilesForHiding(bmpfile, txtfile);

    bfile = fopen(bmpfile,"rb");
    tfile = fopen(txtfile,"rb");
    outfile = fopen(output,"w+b");

    fread(header,54,1,bfile);
    fwrite(header,54,1,outfile);
    bmpFilesize = bmpFilesize - 54;

    while(!feof(tfile)) {
        txtbuffer = fgetc(tfile);
        for(i = 0; i<8; i++) {
            bmpbuffer = fgetc(bfile);
            bmpbuffer &= 0xFE;
            bmpbuffer |= (char)((txtbuffer >> i) & 1);
            fputc(bmpbuffer,outfile);

            bmpTotalStuffablechar--;
            bmpFilesize--;
        }
    }

    for(i = 0; i < 8; i++) {
        bmpbuffer = fgetc(bfile);
        bmpbuffer &= 0xFE;
        bmpbuffer |= (char)((txtTerminatorIndicator >> i) & 1);
        fputc(bmpbuffer,outfile);
        bmpTotalStuffablechar--;
        bmpFilesize--;
    }


    if(bmpFilesize != 0) {
        while( ! feof(bfile) ) {
            fputc(fgetc(bfile), outfile);
        }
    }


    fclose(bfile);
    fclose(tfile);
    fclose(outfile);

    return 0;
}

// encoding implementation...
int BmpFile::unhide (char *bmpfile, char *txtfile)
{
    if(readHeader(bmpfile) == -1)
        return -1;

    FILE *bfile, *tfile;
    char ch, bmpBuffer[8];
    int i;
    bfile = fopen(bmpfile,"rb");
    tfile = fopen(txtfile,"w+b");

    fseek(bfile, 54, SEEK_SET);
    ch = 0;

    while(!feof(bfile)) {

        ch = 0;
        for(i=0; i<=7; i++) {
            bmpBuffer[i] = fgetc(bfile);
        }

        for(i=7; i>=0; i--) {
            ch += (bmpBuffer[i] & 1);
            if(i != 0)
                ch <<= 1;
        }

        if(ch == EOF || ch == '*') {
            break;
        } else {
            fputc(ch,tfile);
        }
    }

    fclose(bfile);
    fclose(tfile);

    return 0;
}


BmpFile::~BmpFile ()
{}

int BmpFile::printFileInfo()
{

    cout<<"identifier      : "<<bmpIdentifier<<endl;
    cout<<"filesize        : "<<bmpFilesize<<endl;
    cout<<"res1            : "<<bmpres1<<endl;
    cout<<"res2            : "<<bmpres2<<endl;
    cout<<"pixoff          : "<<bmpPixoff<<endl;
    cout<<"isize           : "<<bmpiSize<<endl;
    cout<<"Width           : "<<bmpWidth<<endl;
    cout<<"Height          : "<<bmpHeight<<endl;
    cout<<"Plane           : "<<bmpPlanes<<endl;
    cout<<"BitPixel        : "<<bmpBitsPixel<<endl;
    cout<<"compression     : "<<bmpCompression<<endl;
    cout<<"imageSize       : "<<bmpImageSize<<endl;
    cout<<"Xscale          : "<<bmpXscale<<endl;
    cout<<"YScale          : "<<bmpYScale<<endl;
    cout<<"color           : "<<bmpColor<<endl;
    cout<<"ImpCol          : "<<bmpImpCol<<endl;
    cout<<"---------------------------------------"<<endl;
    cout<<"Width           : " << bmpWidth << endl;
    cout<<"Height          : " << bmpHeight << endl;
    cout<<"comprss         : "<< bmpCompression<<endl;
    cout<<"FileSize        : " << bmpFilesize <<endl;
    cout<<"Stuffable ASCII : " << bmpTotalStuffablechar << endl;

    return 0;
}
