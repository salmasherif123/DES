#include <bits/stdc++.h>
#define endl '\n'
using namespace std;
class DES{
    string CipherText;
    string PlainText;
    string userKeyChars;
    vector<string>CipherVec;
public:
    string keysRoundGenerators[16];// Array to save the 16 keys
    string reversedKeysRoundGenerators[16];// Array to save the 16 keys
    DES(){};
    vector<string> inputHandling(string input){
        int length = (int)input.length();
        int size = (int)ceil((double)length/8);
        vector<string>inputArray(size);
        if (length<8){
            for (int i = 0; i < 8 - length; ++i) {
                input+=' ';
            }inputArray[0]=input;
        }
        else if(length>8){
            int leadingZeros = 8-(length%8);
            while (leadingZeros--){
                input+=' ';
            }
            for (int i = 0,j=0; i < length+leadingZeros; i+=8,j++) {
                inputArray[j]=input.substr(i,8);
            }

        }else{
            inputArray[0]=input;
        }
        return inputArray;
    }
    string str2bin(string str){
        int length = (int)str.length();
        string binary8bits;
        for (int i = 0; i < length; ++i) {
            string binary;
            int ch = (unsigned char)str[i];
            while (ch > 0) {
                binary += (char)((ch % 2)+'0');
                ch = ch / 2;
            }
            int bits = (int)binary.length();
            reverse(binary.begin(),binary.end());
            for (int j = 0; j < 8 - bits; ++j) {
                binary8bits+='0';
            }
            binary8bits+=binary;
        }
        return binary8bits;
    }
    string dec2bin(int num){
        string binary;
        string binary8bits;
        while (num>0){
            binary+=(char)((num%2)+'0');
            num/=2;
        }
        reverse(binary.begin(),binary.end());
        int bits = (int)binary.length();
        for (int j = 0; j < 4 - bits; ++j) {
            binary8bits+='0';
        }
        binary8bits+=binary;
        return binary8bits;
    }
    string bin2str(string str){
        int length = (int)str.length();
        string output;
        for (int i = 0; i < length; i+=8) {
            int n = 128,res=0;
            for (int j = i; j < i+8; ++j) {
                res+=(n*(str[j]-'0'));
                n/=2;
            }
            output+=(char)res;
        }
        return output;
    }
    string XOR(string str1,string str2){
        string xoredStr;
        int length = (int)str1.length();
        for (int i = 0; i < length; ++i) {
            if (str1[i]==str2[i])xoredStr+='0';
            else xoredStr+='1';
        }
        return xoredStr;
    }
    string PC_1(string key){
        int pc_1[56] = {
                57,49,41,33,25,17,9,
                1,58,50,42,34,26,18,
                10,2,59,51,43,35,27,
                19,11,3,60,52,44,36,
                63,55,47,39,31,23,15,
                7,62,54,46,38,30,22,
                14,6,61,53,45,37,29,
                21,13,5,28,20,12,4
        };
        string permutedKey;
        for(int i : pc_1){//D-BOX Compressing the key using the PC_1 table from 64-bit to 56-bit
            permutedKey+= key[i-1];
        }
        return permutedKey;
    }
    string PC_2(string CD){
        int pc_2[48] = {
                14,17,11,24,1,5,3,28,
                15,6,21,10,23,19,12,4,
                26,8,16,7,27,20,13,2,
                41,52,31,37,47,55,30,40,
                51,45,33,48,44,49,39,56,
                34,53,46,42,50,36,29,32
        };
        string roundKey;
        for(int i : pc_2){
            roundKey += CD[i-1];
        }
        return roundKey;
    }
    string shiftLeftByOne(string block){
        string keyShiftedOnce;
        for(int i = 1; i < 28; i++){
            keyShiftedOnce += block[i];
        }
        keyShiftedOnce += block[0];
        return keyShiftedOnce;
    }
    string shiftLeftByTwo(string block){
        string keyShiftedTwice;
        for(int i = 0; i < 2; i++){
            for(int j = 1; j < 28; j++){
                keyShiftedTwice += block[j];
            }
            keyShiftedTwice += block[0];
            block= keyShiftedTwice;
            keyShiftedTwice ="";
        }
        return block;
    }
    void keysRoundsGenerate(string key){
        string permuted_key = PC_1(key);
        //  Dividing 56-bit-key resulted from first permutation  into two equal halves each 28 bits
        string leftPacket = permuted_key.substr(0, 28);
        string rightPacket = permuted_key.substr(28);
        // Generating 16 keys for each Encryption stage round
        for(int i=0; i<16; i++){
            //  For rounds 1, 2, 9, 16 the block of key chars
            // are shifted by one.
            if(i == 0 || i == 1 || i==8 || i==15 ){
                leftPacket= shiftLeftByOne(leftPacket);
                rightPacket= shiftLeftByOne(rightPacket);
            }
                //For other rounds,  the block of key chars
                // are shifted by two.
            else{
                leftPacket= shiftLeftByTwo(leftPacket);
                rightPacket= shiftLeftByTwo(rightPacket);
            }
            // The keyBlocks are combined
            string combined_key = leftPacket + rightPacket;
            //  the PC-2 table is used to transpose
            // the key bits
            string round_key = PC_2(combined_key);
            keysRoundGenerators[i]=round_key;
//            cout << "Key " <<i+1 << ": " << keysRoundGenerators[i] << endl;
        }
    }
    void keyGenerationMain(){
        cout<<"please enter the key must be 8-characters"<<endl;
        cin>>userKeyChars;
        cout<<endl;
        while(true){
            if(userKeyChars.length()>8||userKeyChars.length()<8){
                cout<< system("Color 74")<< "ERROR!1 PLEASE ENTER ONLY 8-CHARACTERS!!"<<endl;
                cout<<"please enter the key must be 8-characters  "<<endl;
                cin>>userKeyChars;
            }
            else{
                string key=str2bin(userKeyChars);
                keysRoundsGenerate(key);
                break;
            }

        }
    }
    string shiftRightByOne(string block){// Function make  right shift by 1
        string reversed_keyShiftedOnce[block.length()+1];
        string chars;
        reversed_keyShiftedOnce[0]=block[27];
        for(int i = 0; i < block.length(); i++){
            reversed_keyShiftedOnce[i+1] += block[i];
        }
        for(int i=0;i<block.length();i++){
            chars+= reversed_keyShiftedOnce[i];
        }
        return chars;
    }
    string shiftRightByTwo(string block){
        string reversed_keyShiftedTwice[block.length()+2];
        string chars;
        reversed_keyShiftedTwice[0]=block[26];
        reversed_keyShiftedTwice[1]=block[27];
        for(int i = 0; i < block.length(); i++){
            reversed_keyShiftedTwice[i+2] += block[i];
        }

        for(int i=0;i<block.length();i++){
            chars+= reversed_keyShiftedTwice[i];
        }
        return chars;

    }
    void reverseKeysRoundsGenerate(string key){
        string reversedPermutedKey = PC_1(key);
        //  Dividing 56-bit-key resulted from first permutation  into two equal halves each 28 bits
        string leftPacket = reversedPermutedKey.substr(0, 28);
        string rightPacket = reversedPermutedKey.substr(28);
        reversedKeysRoundGenerators[0] = PC_2(leftPacket+rightPacket);
        // Generating 16 keys for each encryption stage round
        for(int i=1; i<16; i++){
            //  For rounds 1, 2, 9, 16 the block of key chars
            // are shifted by one.
            if(i == 1 || i==8 || i==15 ){
                leftPacket= shiftRightByOne(leftPacket);
                rightPacket= shiftRightByOne(rightPacket);
            }
                //For other rounds,  the block of key chars
                // are shifted by two.
            else{
                leftPacket= shiftRightByTwo(leftPacket);
                rightPacket= shiftRightByTwo(rightPacket);
            }
            // The keyBlocks are combined
            string combinedKey = leftPacket + rightPacket;
            //  the PC-2 table is used to transpose
            // the key bits
            string round_key = PC_2(combinedKey);
            reversedKeysRoundGenerators[i] = round_key;
//            cout << "Key " <<i+1 << ": " << reversedKeysRoundGenerators[i] << endl;
        }
    }
    void reverseKeyGenerationMain(){
        string key=str2bin(userKeyChars);
        reverseKeysRoundsGenerate(key);
    }
    void InitialPermutation(string &plainText){
        string output;
        int IP[]={
                58,50,42,34,26,18,10,2,
                60,52,44,36,28,20,12,4,
                62,54,46,38,30,22,14,6,
                64,56,48,40,32,24,16,8,
                57,49,41,33,25,17,9,1,
                59,51,43,35,27,19,11,3,
                61,53,45,37,29,21,13,5,
                63,55,47,39,31,23,15,7
        };
        for (int i = 0; i < 64; ++i) {
            output+=plainText[IP[i]-1];
        }
        plainText=output;
    }
    string FinalPermutation(string str){
        string output;
        int IPinv[]={
                40,8,48,16,56,24,64,32,
                39,7,47,15,55,23,63,31,
                38,6,46,14,54,22,62,30,
                37,5,45,13,53,21,61,29,
                36,4,44,12,52,20,60,28,
                35,3,43,11,51,19,59,27,
                34,2,42,10,50,18,58,26,
                33,1,41,9,49,17,57,25,
        };
        for (int i : IPinv) {
            output+=str[i-1];
        }
        return output;
    }
    string Expansion(string Ri){
        int ExpansionArray []={
                32,1,2,3,4,5,
                4,5,6,7,8,9,
                8,9,10,11,12,13,
                12,13,14,15,16,17,
                16,17,18,19,20,21,
                20,21,22,23,24,25,
                24,25,26,27,28,29,
                28,29,30,31,32,1
        };
        string ExpandedRi;
        for (int i : ExpansionArray) {
            ExpandedRi+=Ri[i-1];
        }
        return ExpandedRi;
    }
    string SBoxes(string XORedString){
        string output;
        int lookUpTable[8][4][16]={
                {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
                 0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
                 4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
                 15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13
                },
                {15, 1, 8, 14, 6 ,11 ,3 ,4 ,9, 7, 2, 13, 12, 0 ,5 ,10,
                 3, 13, 4, 7, 15, 2, 8, 14 ,12, 0 ,1 ,10 ,6 ,9 ,11 ,5,
                 0, 14 ,7, 11 ,10, 4, 13, 1, 5, 8 ,12, 6 ,9, 3, 2, 15,
                 13, 8 ,10, 1, 3, 15,4 ,2 ,11 ,6 ,7 ,12 ,0 ,5, 14, 9
                },
                {10, 0 ,9 ,14 ,6 ,3 ,15 ,5 ,1 ,13 ,12 ,7 ,11, 4, 2, 8,
                 13, 7 ,0 ,9, 3 ,4 ,6 ,10 ,2 ,8 ,5 ,14 ,12 ,11 ,15 ,1,
                 13 ,6, 4 ,9, 8, 15 ,3 ,0 ,11 ,1 ,2 ,12, 5 ,10 ,14, 7,
                 1 ,10 ,13 ,0 ,6 ,9 ,8 ,7 ,4 ,15, 14 ,3 ,11 ,5 ,2 ,12
                },
                { 7 ,13 ,14 ,3 ,0 ,6, 9 ,10 ,1,2 ,8, 5 ,11 ,12 ,4 ,15,
                  13, 8 ,11 ,5,6 ,15 ,0, 3,4 ,7 ,2 ,12 ,1 ,10 ,14 ,9,
                  10, 6, 9, 0 ,12 ,11 ,7 ,13 ,15, 1 ,3 ,14, 5 ,2, 8, 4,
                  3 ,15 ,0, 6, 10, 1 ,13, 8, 9, 4, 5 ,11, 12, 7, 2, 14
                },
                { 2, 12, 4, 1, 7 ,10, 11 ,6 ,8 ,5 ,3 ,15, 13, 0 ,14, 9,
                  14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
                  4, 2, 1, 11, 10, 13, 7, 8 ,15, 9 ,12 ,5 ,6 ,3 ,0 ,14,
                  11, 8 ,12 ,7 ,1, 14 ,2 ,13, 6, 15, 0 ,9 ,10, 4 ,5 ,3
                },
                { 12, 1 ,10 ,15 ,9, 2 ,6 ,8 ,0 ,13, 3, 4 ,14 ,7 ,5 ,11,
                  10 ,15, 4 ,2, 7, 12, 9, 5 ,6, 1 ,13, 14, 0 ,11, 3, 8,
                  9 ,14, 15, 5 ,2, 8, 12, 3, 7, 0, 4 ,10, 1, 13, 11, 6,
                  4 ,3 ,2, 12, 9 ,5 ,15, 10, 11, 14, 1 ,7 ,6 ,0 ,8 ,13},
                { 4, 11, 2, 14 ,15 ,0, 8 ,13,3 ,12, 9, 7, 5, 10, 6, 1,
                  13, 0, 11 ,7 ,4, 9, 1 ,10, 14, 3 ,5, 12, 2, 15 ,8, 6,
                  1, 4 ,11, 13 ,12 ,3 ,7 ,14 ,10 ,15 ,6 ,8 ,0 ,5 ,9 ,2,
                  6 ,11, 13, 8 ,1 ,4, 10 ,7 ,9 ,5 ,0 ,15 ,14 ,2 ,3 ,12 },
                { 13 ,2 ,8 ,4 ,6 ,15, 11, 1 ,10, 9, 3, 14, 5 ,0 ,12 ,7,
                  1, 15, 13 ,8, 10, 3 ,7, 4, 12, 5, 6, 11 ,0 ,14 ,9, 2,
                  7 ,11 ,4 ,1 ,9, 12, 14, 2, 0, 6, 10, 13 ,15, 3, 5, 8,
                  2 ,1 ,14 ,7 ,4 ,10 ,8 ,13, 15, 12 ,9, 0 ,3 ,5 ,6 ,11 }
        };
        for (int i = 0,j=0; i < 48; i+=6,j++) {
            int column = (XORedString[i+1]-'0')*8 + (XORedString[i+2]-'0')*4 + (XORedString[i+3]-'0')*2 + (XORedString[i+4]-'0');
            int row = (XORedString[i]-'0')*2 + (XORedString[i+5]-'0');
            output+= dec2bin(lookUpTable[j][row][column]);
        }
        return output;
    }
    string permutation(string str){
        string permutedStr;
        int P[]={16 ,7 ,20, 21 ,29 ,12,28 ,17,
                 1 ,15, 23, 26, 5, 18 ,31 ,10,
                 2 ,8 ,24 ,14 ,32 ,27 ,3 ,9,
                 19 ,13 ,30 ,6 ,22 ,11 ,4 ,25
        };
        for (int i : P) {
            permutedStr+=str[i-1];
        }
        return permutedStr;
    }
    string Function(string Ri, string key){
        string E = Expansion(Ri);
        string Xor = XOR(E,key);
        string S = SBoxes(Xor);
        return permutation(S);
    }
    string Rounds(string binaryPlainText,string keys[]){
        InitialPermutation(binaryPlainText);
        string Lo = binaryPlainText.substr(0,32);
        string Ro = binaryPlainText.substr(32);
        vector<pair<string,string>>LR;
        LR.emplace_back(Lo,Ro);
        for (int i = 1; i <=16; ++i) {
            string F = Function(LR[i - 1].second, keys[i - 1]);
            string R = XOR(LR[i-1].first, F);
            LR.emplace_back(LR[i-1].second,R);
        }
        return FinalPermutation(LR[16].second+LR[16].first);
    }
    void Encryption(){
        cout<<"Enter your plain text : "<<endl;
        getline(cin,PlainText);
        vector<string>input = inputHandling(PlainText);
        int length = (int)input.size();
        keyGenerationMain();
        cout<<"Plain text:\n"<<PlainText<<endl;
        string blockOfPlainText,blockOfCipherText;
        for (int i = 0; i < length; ++i) {
            blockOfPlainText = str2bin(input[i]);
            blockOfCipherText=Rounds(blockOfPlainText, keysRoundGenerators);
            CipherVec.push_back(blockOfCipherText);
            CipherText+=blockOfCipherText;
        }
        cout<<"\nPlain text in binary:\n"<<str2bin(PlainText)<<endl;
        cout<<"\nCipher text:\n"<<bin2str(CipherText)<<endl;
        cout<<"\nCipher text in binary:\n"<<CipherText<<endl;
    }
    void Decryption(){
        reverseKeyGenerationMain();
        string decryptedText;
        int length = (int)CipherVec.size();
        for (int i = 0; i < length; ++i) {
            decryptedText+=Rounds(CipherVec[i], reversedKeysRoundGenerators);
        }
        string binaryDecryptedText = decryptedText;
        decryptedText=bin2str(decryptedText);
        int decryptedTextLength = (int)decryptedText.length();
        int specialCharCounter=0;
        for (int i =0; i < 8 ; ++i) {
            if (decryptedText[decryptedTextLength-1-i]==' ')
                specialCharCounter++;
        }
        cout<<"\nDecrypted (Plain) text in binary:\n"<<binaryDecryptedText<<endl;
        cout<<"\nDecrypted (plain) text :\n"<<decryptedText.substr(0,decryptedTextLength-specialCharCounter)<<endl;
    }
};
int main() {
      DES des;
      des.Encryption();
      des.Decryption();
    return 0;
}

