#include<bits/stdc++.h>
using namespace std;

struct Author {
    char authorID[15];
    char authorName[30];
    char address[30];
};

struct Book {
    char ISBN[15];
    char bookTitle[30];
    char authorID[15];
};

void updatePI(const string &file, const string &index);

void updatePI2(const string &file, const string &index);

void updateBooksSI();

void updateAuthorsSI();

void readAuthors();

void Add(string type);

pair<bool, pair<string, string> > binarySearch(const vector<pair<string, string> > &index, const int &id);

pair<bool, pair<string, string> > binarySearch(const vector<pair<string, string> > &index, const string &id);

string search(int bitOffset, const string &file);

int searchPI(string id, const string &file, const string &file2);

vector<int> searchSI(string id, const string &SIfile, const string &file, const string &PIfile, const string &LL);

bool deleteRecordPI(string id, string filePI);

bool deleteAuthorSI(string id, string secondaryKey , string fileSI, string fileLL);

void deleteRecord(char id[], string originalFile, string filePI, string fileSI, string fileLL, string type);

void deleteHandler(string recordType);

void updateAuthorName();

void updateBookTitle();

void writeQuery();
void printAuthor();
void printBook();
short cntID = 0, CntNameLL = 0, CntNameSec = 0;

int main() {
    fstream author("authors.txt", ios::in|ios::out|ios::binary);
    author.seekg(0, ios::beg);
    short head=-1;
    author.write((char*)&head, sizeof(head));
    author.close();
    fstream book("books.txt", ios::in|ios::out|ios::binary);
    book.seekg(0, ios::beg);
    book.write((char*)&head, sizeof(head));
    book.close();

    while (true) {
        cout << "Welcome to the Library Catalog System\n";
        cout << "1. Add New Author\n";
        cout << "2. Add New Book\n";
        cout << "3. Update Author Name (Author ID)\n";
        cout << "4. Update Book Title (ISBN)\n";
        cout << "5. Delete Book (ISBN)\n";
        cout << "6. Delete Author (Author ID)\n";
        cout << "7. Print Author (Author ID)\n";
        cout << "8. Print Book (ISBN)\n";
        cout << "9. Write Query\n";
        cout << "10. Exit\n";

        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                Add("Author");
                break;
            }
            case 2: {
                Add("Book");
                break;
            }
            case 3: {
                updateAuthorName();
                break;
            }
            case 4: {
                updateBookTitle();
                break;
            }
            case 5: {
                deleteHandler("Book");
                break;
            }
            case 6: {
                deleteHandler("Author");
                readAuthors();
                break;
            }
            case 7: {
                void printAuthor();
                break;
            }
            case 8: {
                void printBook();
                break;
            }
            case 9: {
                writeQuery();
                break;
            }
            case 10: {

                return 0;
            }
            default: {
                cout << "Invalid choice. Please try again.\n";
                break;
            }
        }
    }
}


void updatePI(const string &file, const string &index) {

    ifstream dataFile(file);
    ofstream indexFile(index);

    //check that datafile is full
    if (!dataFile.is_open() || !indexFile.is_open()) {
        cerr << "Error opening files." << endl;
        return;
    }

    //declaration
    vector<pair<int, int> > primaryIndex;
    string id;
    pair<int, int> p;
    short bitOffset = 2;

    //set the cursor at the beginning of the file skipping first 2 char
    dataFile.seekg(bitOffset, ios::beg);

    //getting the length indicator of the record
    short lengthIndicator;
    dataFile.read((char*)&lengthIndicator, 2);
    int recLen = lengthIndicator;
    bitOffset += 2;

    while (!dataFile.eof()) {

        getline(dataFile, id, '|');

        //adding the id and the bit offset of the record to the vector
        p.first = stoi(id);
        p.second = bitOffset;
        primaryIndex.push_back(p);

        //adding length of record to the bit offset
        bitOffset += 2;
        bitOffset += recLen;

        //getting the length indicator of the next record
        dataFile.seekg((recLen - id.length()-1), ios::cur);
        dataFile.read((char*)&lengthIndicator, 2);
        recLen = lengthIndicator;

    }

    sort(primaryIndex.begin(), primaryIndex.end());
    //filling the index file
    for (const auto &entry: primaryIndex) {
        indexFile << entry.first << '|' << entry.second << "\n";
    }

    dataFile.close();
    indexFile.close();
}

void updatePI2(const string &file, const string &index) {

    ifstream dataFile(file);
    ofstream indexFile(index);

    //check that datafile is full
    if (!dataFile.is_open() || !indexFile.is_open()) {
        cerr << "Error opening files." << endl;
        return;
    }

    //declaration
    vector<pair<int, int> > primaryIndex;
    string id;
    pair<int, int> p;
    int bitOffset = 2;

    //set the cursor at the beginning of the file skipping first 2 char
    dataFile.seekg(bitOffset, ios::beg);

    //getting the length indicator of the record
    short lengthIndicator;
    dataFile.read((char*)&lengthIndicator, 2);
    int recLen = lengthIndicator;
    bitOffset += 2;

    while (!dataFile.eof()) {

        getline(dataFile, id, '|');


        //adding the id and the bit offset of the record to the vector
        p.first = stoi(id);
        p.second = bitOffset;
        primaryIndex.push_back(p);

        //adding length of record to the bit offset
        bitOffset += 2;
        bitOffset += recLen;

        //getting the length indicator of the next record
        dataFile.seekg((recLen - id.length() - 1), ios::cur);
        dataFile.read((char*)&lengthIndicator, 2);
        recLen = lengthIndicator;

    }

    sort(primaryIndex.begin(), primaryIndex.end());
    //filling the index file
    for (int i = 0; i < primaryIndex.size(); i++) {
        indexFile << primaryIndex[i].first << '|' << primaryIndex[i].second ;
        if (i <primaryIndex.size() - 2) indexFile << "\n";
    }

    dataFile.close();
    indexFile.close();
}

void updateBooksSI() {

    ifstream dataFile("books.txt");
    ofstream indexFile("booksSI.txt");

    //check that datafile is full
    if (!dataFile.is_open() || !indexFile.is_open()) {
        cerr << "Error opening files." << endl;
        return;
    }

    //declaration
    vector<pair<int, string> > secondaryIndex;
    string id, bookTitle;
    pair<int, string> p;



    //set the cursor at the beginning of the file skipping first 2 char
    dataFile.seekg(2, ios::beg);

    //getting the length indicator of the record
    short lengthIndicator;
    dataFile.read((char*)&lengthIndicator, 2);
    int recLen = lengthIndicator;


    while (!dataFile.eof()) {
        int bitoffset = recLen;
        getline(dataFile, id, '|');
        getline(dataFile, bookTitle, '|');
        bitoffset -= id.length();
        bitoffset -= bookTitle.length();
        bitoffset -= 2;
        char buffer[bitoffset];
        dataFile.read(buffer, bitoffset);
        string authorID(buffer);

        //adding the id and the bit offset of the record to the vector

        bool added = false;
        for (auto &i: secondaryIndex) {
            if (authorID == to_string(i.first)) {
                i.second += ',';
                i.second += id;
                added = true;
            }
        }

        if (!added) {
            p.first = stoi(authorID);
            p.second = id;
            secondaryIndex.push_back(p);
        }

        //adding length of record to the bit offset


        //getting the length indicator of the next record
        dataFile.read((char*)&lengthIndicator, 2);
        recLen = lengthIndicator;

    }

    sort(secondaryIndex.begin(), secondaryIndex.end());
    //filling the index file
    for (const auto &entry: secondaryIndex) {
        indexFile << entry.first << '|' << entry.second << "\n";
    }

    dataFile.close();
    indexFile.close();
}

void updateAuthorsSI() {

    ifstream dataFile("authors.txt");
    ofstream indexFile("authorSI.txt");

    //check that datafile is full
    if (!dataFile.is_open() || !indexFile.is_open()) {
        cerr << "Error opening files." << endl;
        return;
    }

    //declaration
    vector<pair<string, string> > secondaryIndex;
    string id, authorName;
    pair<string, string> p;



    //set the cursor at the beginning of the file skipping first 2 char
    dataFile.seekg(2, ios::beg);

    //getting the length indicator of the record
    short lengthIndicator;
    dataFile.read((char*)&lengthIndicator, 2);
    int recLen = lengthIndicator;


    while (!dataFile.eof()) {

        getline(dataFile, id, '|');
        getline(dataFile, authorName, '|');


        //adding the id and the bit offset of the record to the vector

        bool added = false;
        for (auto &i: secondaryIndex) {
            if (authorName == i.first) {
                i.second += ',';
                i.second += id;
                added = true;
            }
        }

        if (!added) {
            p.first = authorName;
            p.second = id;
            secondaryIndex.push_back(p);
        }

        //adding length of record to the bit offset


        //getting the length indicator of the next record
        dataFile.seekg(recLen - 3 - authorName.length(), ios::cur);
        dataFile.read((char*)&lengthIndicator, 2);
        recLen = lengthIndicator;

    }

    sort(secondaryIndex.begin(), secondaryIndex.end());
    //filling the index file
    for (const auto &entry: secondaryIndex) {
        indexFile << entry.first << '|' << entry.second << "\n";
    }

    dataFile.close();
    indexFile.close();
}
pair<bool, pair<string, string>> binarySearch(const vector<pair<string, string>> &index, const string &id) {
    int ID = stoi(id);
    int low = 0;
    int high = index.size() - 1;
    pair<bool, pair<string, string>> result;
    result.first = false;

    while (low <= high) {
        int mid = (low + high) / 2;
        if (stoi(index[mid].first) == ID) {
            result.first = true;
            result.second = index[mid];
            break;
        } else if (stoi(index[mid].first) < ID) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return result;
}

pair<bool, pair<string, string> > binarySearch(const vector<pair<string, string> > &index, const int &id) {

    int low = 0;
    int high = index.size() - 1;
    pair<bool, pair<string, string> > result;
    result.first = false;

    while (low <= high) {
        int mid = (low + high) / 2;
        if (stoi(index[mid].first) == id) {
            result.first = true;
            result.second = index[mid];
            break;
        } else if (stoi(index[mid].first) < id) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return result;


}

int searchPI(string id, const string &file, const string &file2) {
    ifstream indexFile(file);
    ifstream File(file2);

    //check that datafile is full
    if (!indexFile.is_open()) {
        cerr << "Error opening files." << endl;
    }

    indexFile.seekg(0, ios::beg);
    vector<pair<string, string> > indx;
    pair<string, string> entry;
    while (getline(indexFile, entry.first, '|')&& getline(indexFile, entry.second)) {
        indx.push_back(entry);
    }
    auto x = binarySearch(indx, stoi(id) );
    if (x.first) {
        int bitoffset = stoi(x.second.second);
        return bitoffset;
    } else return -1;

    indexFile.close();
    File.close();

}


vector<int> searchSI(string id, const string &SIfile, const string &file, const string &PIfile, const string &LL) {
    fstream SIFile(SIfile);
    fstream PIFile(PIfile);
    fstream File(file);
    fstream LLFile(LL);

    //check that datafile is full
    if (!SIFile.is_open() || !PIFile.is_open() || !LLFile.is_open() || !File.is_open()) {
        cerr << "Error opening files." << endl;

    }

    SIFile.seekg(0, ios::beg);
    vector<pair<string, string> > indx;
    pair<string, string> entry;
    while (!SIFile.eof()) {

        std::getline(SIFile, entry.first, '|');
        SIFile >> entry.second;
        indx.push_back(entry);
        SIFile.ignore();  // Ignore the newline character
    }
    auto x = binarySearch(indx, id);

    vector<int> result;
    string key, value;
    if (x.first) {
        int offset = stoi(x.second.second);
        while (offset != -1) {
            LLFile.seekg(offset, ios::beg);
            getline(LLFile, key, '|');
            getline(LLFile, value, ' ');
            result.push_back(searchPI(key, PIfile, file));
            offset = (stoi(value));

        }


    }
    return result;

    PIFile.close();
    SIFile.close();
    File.close();

}

string search(int bitOffset, const string &file) {

    ifstream dataFile(file);

    //check that datafile is full
    if (!dataFile.is_open()) {
        cerr << "Error opening files." << endl;
    }

    //set the cursor at the beginning of the record and subtract 2 to get the length indicator
    dataFile.seekg(bitOffset - 2, ios::beg);

    //getting the length indicator of the record
    short lengthIndicator;
    dataFile.read((char*)&lengthIndicator, 2);
    int recLen = lengthIndicator;

    string rec;
    for (int i = 0; i < recLen; i++) {
        rec += dataFile.get();
    }
    return rec;
    dataFile.close();
}

void readAuthors() {
    ifstream file("authors.txt", ios::in|ios::out);

    if (!file.is_open()) {
        cout << "Error: opening file failed\n";
        return;
    }

    short header;
    file.read((char*)&header, sizeof(header));
    cout << "Header: "<< header << endl;

    short size;
    file.read((char*)&size, sizeof(size));

    while (file) {
        char record[size];
        file.read(record, size);
        char mark = record[0];

        if (mark != '*') {

            string authorID, authorName, address;

            int fieldCounter = 0;
            for(int i  = 0; i < size;i++){
                if (record[i]=='|') {
                    fieldCounter++;
                    continue;
                }
                if (fieldCounter==0){
                    authorID+=record[i];
                }
                if(fieldCounter==1){
                    authorName+=record[i];
                }
                if(fieldCounter==2){
                    address+=record[i];
                }
            }
            cout << authorID << "|" << authorName << "|" << address << endl;
        }
        file.read((char*)&size, sizeof(size));

    }

    file.close();
}

bool deleteRecordPI(string id, string filePI){
    // load primary index on vector of pairs <id, offset>
    fstream file(filePI, ios::in|ios::out|ios::binary);
    vector<pair<string,  string> > primaryIndex;
    pair<string, string> entry;

    while (getline(file, entry.first, '|') && getline(file,entry.second)) {
        primaryIndex.push_back(entry);
    }

    file.close();

    // search using binary search
    int low = 0, high = primaryIndex.size() - 1, mid=-1;
    while(low <= high){
        mid = (high + low) / 2;
        if(primaryIndex[mid].first < id){
            low = mid + 1;
        } else if (primaryIndex[mid].first > id){
            high = mid - 1;
        } else {
            break;
        }
    }

    if(low > high ) return false;

    // once found authorId delete entry
    primaryIndex.erase(primaryIndex.begin() + mid);

    // rewrite index to file
    file.open(filePI, ios::trunc|ios::out);
    if(!file.is_open()){
        cout << "Error: failed to open index file\n";
        return false;
    }

    for(int i = 0; i < primaryIndex.size(); i++){
        file << primaryIndex[i].first << '|' << primaryIndex[i].second;
        if(i < primaryIndex.size()-1 )
            file << "\n";
    }

    file.close();
    return true;
}

bool deleteAuthorSI(string id, string secondaryKey , string fileSI, string fileLL){
    // load secondary index
    fstream file(fileSI, ios::in|ios::out|ios::binary);
    vector<pair<string,  string> > secondaryIndex;
    pair<string, string> entry;

    if(!file.is_open()){
        cout << "Fail to open SI file\n";
        return false;
    }

    while (getline(file, entry.first, '|') && getline(file, entry.second)) {
        secondaryIndex.push_back(entry);
    }

    file.close();
    short offset = -1;
    // search for name or author id
    int i;
    for (i = 0; i < secondaryIndex.size(); i++) {

        if (secondaryIndex[i].first == secondaryKey) { // extract offset
            offset = stoi(secondaryIndex[i].second);
//            cout << "Offset: " << offset << endl; // TODO remove
//            cout << "Found!" << endl; // TODO remove
            break;
        }
    }


    if(offset ==-1){
        cout << "secondary key (not found) in secondary index\n";
        return false;
    } else {
        cout << "secondary key (found) in secondary index\n";
    }
    // go to linked list
    fstream linkedlistFile(fileLL, ios::in|ios::out|ios::binary);
    if(!linkedlistFile.is_open()){
        cout << "Fail to open linked list file(SI)\n";
        return false;
    }

    short prev, curr = offset, next, mark=-2;
//        cout << "offset" << offset<<endl; // TODO remove
    bool found = false;
    linkedlistFile.seekg(curr, ios::beg);
    string ID;
    getline(linkedlistFile, ID, '|');

    if(ID == id){
        linkedlistFile.read((char*)&next, 2);
        secondaryIndex[i].second = to_string(next);

        linkedlistFile.seekp(-2, ios::cur);
        linkedlistFile.write((char*)&mark, 2);
        found= true;

    }

    while(!found and curr != -1){
        linkedlistFile.read((char*)&next, 2);

        linkedlistFile.seekg(next, ios::beg);
        getline(linkedlistFile, ID, '|');


        prev = curr;
        curr = next;
        if(ID == id){ // edit links

            linkedlistFile.read((char*)&next, 2);
            linkedlistFile.seekg(prev, ios::beg);
            getline(linkedlistFile, ID, '|');
            linkedlistFile.seekp(0, ios::cur);
            linkedlistFile.write((char*)&next, 2);

            linkedlistFile.seekp(curr, ios::beg);
            getline(linkedlistFile, ID, '|');
            linkedlistFile.seekp(0, ios::cur);
            linkedlistFile.write((char*)&mark, 2);
            found= true;
            break;
        }

    }

    file.close();
    linkedlistFile.close();

    file.open(fileSI, ios::trunc|ios::out|ios::binary);
    for (int i = 0; i < secondaryIndex.size(); i++) {
        file << secondaryIndex[i].first;
        file << '|';
        file << secondaryIndex[i].second;

        if (i < secondaryIndex.size() - 1)
            file << '\n';
    }

    if (!found){
        cout << "Error in finding (id) in the linked list of secondary index\n";
        return false;
    } else{
        cout << "Successfully deleted from linked list\n";
        return true;
    }
}


void deleteRecord(char id[], string originalFile, string filePI, string fileSI, string fileLL, string type){
    fstream file(originalFile, ios::out | ios::in | ios::binary);

    if(!file.is_open()){
        cout << "Deletion failed -> fail to open file\n";
        return;
    }


    // call search function to return the offset of that record if exists
    short offset = searchPI(id, filePI, originalFile); // add here search call instead
//    cout << "from delete "<<offset <<endl;  // TODO remove
    if (offset == -1) {
        cout << "Deletion failed -> id not found\n";
        file.close();
        return;
    }

    string field1, field2, field3, target;
    file.seekg(offset, ios::beg);
    if (type == "Author"){
        getline(file, field1, '|'); // id
        getline(file, field2, '|'); // name
        target = field2;
    } else if (type == "Book"){
        getline(file, field1, '|' ); // isbn
        getline(file, field2, '|' ); // book title
        getline(file, field3, '|');  // author id
        target = field3;
    } else {
        cout << "Deletion failed -> undefined record type\n";
        file.close();
        return;
    }

    // send author name = field 2 or author id (target means : secondary key)
    if(!deleteAuthorSI(id, target, fileSI, fileLL)){
        cout << "Deletion failed -> error in SI\n";
        file.close();
        return;
    }

    if(!deleteRecordPI(id, filePI)){
        cout << "Deletion failed -> error in PI\n";
        file.close();
        return;
    }

    // back to Header to find the start of available list
    short header, deletedRecordSize, currSize;
    file.seekg(0, ios::beg);
    file.read((char*)&header, sizeof(header));

    file.seekg(offset-2, ios :: beg); // length indicator of deleted record
    file.read((char*)&deletedRecordSize, sizeof(deletedRecordSize));

    // not the first record to delete from file
    if(header != -1)
    {
        file.seekg(header - 2, ios::beg); // extract size of first record in avail list (size of first record in avail list (worst fit): largest size)
        file.read((char*)&currSize, sizeof(currSize));
    }

    // empty avail list or deleted record size is the largest
    // empty => header = deletedRecordOffset and deletedRecord: overwrite the first 3 bytes by *-1 (header = -1)
    // currSize <= deletedRecordSize => header = deletedRecordOffset and deletedRecord: overwrite the first 3 bytes by *currOffset as (header = currOffset)
    if(header == -1 or currSize<=deletedRecordSize){
        file.seekp(offset, ios::beg);
        file.write("*", 1);
        file.write((char*)&header, sizeof(header)); // let the deleted record points to the second largest
        file.seekg(0, ios::beg);
        file.write((char*)&offset, sizeof(offset));
        file.close();
        return;
    }

    short currOffset = header, nextOffset, nextSize;
    file.seekg(currOffset+1 , ios::beg); // skip * to read the next offset
    file.read((char*)&nextOffset , sizeof(short));

    // only one item in avail list and its size is >= deleted item size
    // let curr on avail list points to deleted item
    if(nextOffset == -1 and currSize >= deletedRecordSize){
        file.seekp(currOffset+1, ios::beg); // skip * to read the next offset
        file.write((char*)&offset, sizeof(offset));
        file.seekp(offset, ios::beg);
        file.write("*", 1);
        file.write((char*)&nextOffset, sizeof(nextOffset));
        file.close();
        return;
    }

    file.seekg(nextOffset-2, ios::beg);   // extract size of next item
    file.read((char*)&nextSize , sizeof(nextSize));

    while(true) {
        // right place for deleted item is found
        if(nextSize <= deletedRecordSize){
            file.seekp(currOffset+1, ios::beg); // skip * to write the next offset of currOffset
            file.write((char*)&offset, sizeof(offset));
            file.seekp(offset, ios::beg);
            file.write("*", 1); // overwrite one byte by *
            file.write((char*)&nextOffset, sizeof(nextOffset)); // overwrite 2 bytes by next offset of deletedOffset
            file.close();
            return;
        }
        else
        {
            currOffset = nextOffset; // advance the curr to next
            file.seekp(nextOffset+1, ios::beg);
            file.read((char*)&nextOffset , sizeof(nextOffset));

            if(nextOffset == -1){ // reach the end of avail list
                file.seekp(currOffset + 1, ios::beg); // skip * to write the next offset of currOffset
                file.write((char*)&offset, sizeof(offset));
                file.seekp(offset, ios::beg);
                file.write("*", 1);
                file.write((char*)&nextOffset, sizeof(nextOffset));
                break;
            }

            file.seekg(nextOffset-2, ios::beg); // extract size of next
            file.read((char*)&nextSize , sizeof(nextSize));
        }
    }
    file.close();
}

void deleteHandler(string recordType){
    char id[15];
    string message=(recordType=="Author")?"ID":"ISBN";
    string originalFile = (recordType=="Author")?"authors.txt":"books.txt";
    string filePI = (recordType=="Author")?"authorsPI.txt":"booksPI.txt";
    string fileSI = (recordType=="Author")?"authorsSI.txt":"booksSI.txt";
    string fileLL = (recordType=="Author")?"nameLL.txt":"idLL.txt"; // TODO check team about the name of file

    cout << "Enter "<< message << ":";
    cin >> id;

    deleteRecord(id, originalFile, filePI, fileSI, fileLL, recordType);

}

void updateAuthorName() {

    ifstream dataFile("authors.txt");

    if (!dataFile.is_open()) {
        cerr << "Error opening the file." << endl;
        return;
    }

    string authorID;
    cout << "Please enter the author ID you want to change: ";
    cin >> authorID;

    // Search for the author using authorID in the primary index
    int bitOffset = searchPI(authorID, "authorsPI.txt", "authors.txt");
    if (bitOffset != -1) {
        cout << "Author found!"<< endl;
        // Read the existing author record
        string authorRecord = search(bitOffset, "authors.txt");
        cout << "Here's the current author record: " << authorRecord << endl;
        int firstPos = authorRecord.find('|');
        int lastPos = authorRecord.rfind('|');
        int sizeRecord = authorRecord.length();

        // Ask the user for the new author name
        string newAuthorName;
        cout << "Enter the new author name: ";
        cin.ignore(); // Clear the newline character from the buffer
        getline(cin, newAuthorName);

        string recordPrefix = authorRecord.substr(0, firstPos + 1);
        string recordSuffix = authorRecord.substr(lastPos);
        string newAuthorRecord = recordPrefix + newAuthorName + recordSuffix;
        newAuthorRecord = to_string(newAuthorRecord.length()) + newAuthorRecord;
        string stringSize = to_string(sizeRecord);
        authorRecord =stringSize +authorRecord;
        // Update author file
        string allAuthors;
        getline(dataFile, allAuthors);
        dataFile.close();
        ofstream writeFile("authors.txt", ios::out | ios::trunc | ios::binary);
        allAuthors.replace(bitOffset - stringSize.length(), authorRecord.length(), newAuthorRecord);
        writeFile << allAuthors;
        writeFile.close();
        cout << "Author name updated successfully!" << endl;
        // Update the index
        updatePI("authors.txt", "authorsPI.txt");
        updateAuthorsSI();

    }
    else{
        cout << "Author ID not found." << endl;
    }
}

void updateBookTitle() {
    ifstream dataFile("books.txt");

    if (!dataFile.is_open()) {
        cerr << "Error opening the file." << endl;
        return;
    }

    string ISBN;
    cout << "Please enter the ISBN of the book you want to change: ";
    cin >> ISBN;
    cout <<"SearchPI"<<endl;
    // Search for the book using ISBN in the primary index
    int bitOffset = searchPI(ISBN, "booksPI.txt", "books.txt");
    if(bitOffset != -1){
        cout << "Book found!"<< endl;
        // Read the existing Book record
        string bookRecord = search(bitOffset, "books.txt");
        cout << "Here's the current book record: " << bookRecord << endl;
        int firstPos = bookRecord.find('|');
        int lastPos = bookRecord.rfind('|');
        int sizeRecord = bookRecord.length();

        // Ask the user for the new book title
        string newBookTitle;
        cout << "Enter the new book title: ";
        cin.ignore(); // Clear the newline character from the buffer
        getline(cin, newBookTitle);

        string recordPrefix = bookRecord.substr(0, firstPos + 1);
        string recordSuffix = bookRecord.substr(lastPos);
        string newBookRecord = recordPrefix + newBookTitle + recordSuffix;
        newBookRecord = to_string(newBookRecord.length()) + newBookRecord;
        string stringSize = to_string(sizeRecord);
        bookRecord = stringSize + bookRecord;

        // Update book file
        string allBooks;
        getline(dataFile, allBooks);
        dataFile.close();
        ofstream writeFile("books.txt", ios::out | ios::trunc | ios::binary);
        allBooks.replace(bitOffset - stringSize.length(), bookRecord.length(), newBookRecord);
        writeFile << allBooks;
        writeFile.close();
        cout << "Book title updated successfully!" << endl;
        // Update the index
        updatePI("books.txt", "booksPI.txt");
        updateBooksSI();
    }
    else{
        cout << "Book ISBN not found!" << endl;
    }
}

void writeQuery(){

}

void insertInPrimary(const char id[],short offset, string filePI){

    fstream authorsPI(filePI,ios::in|ios::out);
    vector<pair<string,string>> v;
    string s;
    string x;
    while(getline(authorsPI, s, '|') && getline(authorsPI, x)){
        v.push_back(make_pair(s,x));
    }
    v.push_back(make_pair(id,to_string(offset)));
    sort(v.begin(),v.end());
    authorsPI.close();
    authorsPI.open(filePI,ios::in|ios::out|ios::trunc);
    for(int i=0;i<v.size();i++){
        authorsPI<<v[i].first<<'|'<<v[i].second;
        if(i<v.size()-1){
            authorsPI<<'\n';
        }
    }
    authorsPI.close();
}
void insertName(char name[],char id[],string fileSI,string fileLL){
    //first check if the name is already in the secondary index
    //if it is then add the id to the file nameLL.txt and make the id with the same name refers to the offset of this id
    //
    fstream authorSI(fileSI,ios::in|ios::out);
    //check if this name is in the file
    string s1;
    string x1;
    string ID;
    vector<pair<string,string>> v;
    while(getline(authorSI, s1, '|') && getline(authorSI, x1)){
        v.push_back(make_pair(s1,x1));
    }
    string s;
    string x;
    bool found=false;
    while(authorSI.good()){
        getline(authorSI,s,'|');
        authorSI>>x;
        short off=stoi(x);
        authorSI.ignore();
        if(strcmp(s.c_str(),name)==0) {
            found = true;
            //add the id to the file nameLL.txt and make the id with the same name refers to the offset of this id
            fstream nameLL(fileLL, ios::in | ios::out);
            //i want to add the id to the end of the file and make the offset of this id -1 and make the offset of the previous id with the same name refers to the offset of this id
            nameLL.seekg(0, ios::end);
            short offset = nameLL.tellp();
            if (off == -1) {
                for (int i = 0; i < v.size(); i++) {
                    if (v[i].first == name) {
                        v[i].second = to_string(off);
                        break;
                    }
                }
                nameLL.write(id, strlen(id));
                //delimiter
                char c = '|';
                nameLL.write(&c, sizeof(c));
                short nega = -1;
                nameLL.write((char *) &nega, sizeof(nega));

            }
            else{
                //seek to the x in the file nameLL
                nameLL.seekg(off, ios::beg);
                getline(nameLL, ID, '|');
                short of;
                nameLL.read((char *) &of, sizeof(of));
                while (of != -1) {
                    nameLL.seekg(of, ios::beg);
                    getline(nameLL, ID, '|');
                    nameLL.read((char *) &of, sizeof(of));
                }
                nameLL.seekp(-2, ios::cur);
                nameLL.write((char *) &offset, sizeof(offset));
                nameLL.seekp(0, ios::end);
                nameLL.write(id, strlen(id));
                //delimiter
                char c = '|';
                nameLL.write(&c, sizeof(c));
                short nega = -1;
                nameLL.write((char *) &nega, sizeof(nega));
            }
            nameLL.close();
            break;
        }
    }
    if(!found) {
        //if the name is not in the file then add it to the file and add the id to the file nameLL.txt and make the id with the same name refers to the offset of this id
        authorSI.close();
        authorSI.open(fileSI, ios::in | ios::out);
        //get the offset of the last record in the nameLL file
        fstream nameLL(fileLL, ios::in | ios::out);
        nameLL.seekg(0, ios::end);
        short offset = nameLL.tellp();
        nameLL.write(id, strlen(id));
        //delimiter
        char c = '|';
        nameLL.write(&c, sizeof(c));
        short nega = -1;
        nameLL.write((char *) &nega, sizeof(nega));
        //id|offset
        v.push_back(make_pair(name, to_string(offset)));
        authorSI.close();
        nameLL.close();
    }
    //sort the vector
    sort(v.begin(),v.end());
    //write the vector to the file
    authorSI.open(fileSI,ios::in|ios::out|ios::trunc);
    for(int i=0;i<v.size();i++){
        authorSI<<v[i].first<<'|'<<v[i].second;
        if(i<v.size()-1){
            authorSI<<'\n';
        }
    }
    authorSI.close();
}
void Add(string type){
    string OriginalFile;
    string PIFile;
    string SIFile;
    string LLFile;
    if(type=="Author"){
        OriginalFile="authors.txt";
        PIFile="authorsPI.txt";
        SIFile="authorsSI.txt";
        LLFile="nameLL.txt";
    }
    else if(type=="Book"){
        OriginalFile="books.txt";
        PIFile="booksPI.txt";
        SIFile="booksSI.txt";
        LLFile="idLL.txt";
    }
    //get the data from the user
    Author author{};
    if(type=="Author") {
        cout << "Enter the author ID: ";
        cin >> author.authorID;
        cout << "Enter the author name: ";
        cin.ignore();
        cin.getline(author.authorName, 30);
        cout << "Enter the author address: ";
        cin.getline(author.address, 30);
        if(searchPI(author.authorID,PIFile,OriginalFile)!=-1){
            cout<<"This ID is already in the file"<<endl;
            return;
        }
        insertName(author.authorName,author.authorID,SIFile,LLFile);
    }
    else{
        cout << "Enter the ISBN: ";
        cin >> author.authorID;
        cout << "Enter the book title: ";
        cin.ignore();
        cin.getline(author.authorName, 30);
        cout << "Enter the author ID: ";
        cin.getline(author.address, 15);
        if(searchPI(author.authorID,PIFile,OriginalFile)!=-1){
            cout<<"This ID is already in the file"<<endl;
            return;
        }
        insertName(author.address,author.authorID,SIFile,LLFile);
    }
    fstream out(OriginalFile,ios::in|ios::out|ios::binary);
    short RecordSize,IDSize,NameSize, AddressSize;
    short header;
    out.seekg(0,ios::beg);
    out.read((char*)&header,sizeof(header));
    NameSize=strlen(author.authorName);
    AddressSize=strlen(author.address);
    IDSize=strlen(author.authorID);
    RecordSize=IDSize+NameSize+AddressSize +3;
    short unique;
    if(header==-1){
        out.seekp(0,ios::end);
        unique=out.tellp();
        out.write((char*)&RecordSize,sizeof(RecordSize));
        out.write((char*)&author.authorID,IDSize);
        //delimiter
        char c='|';
        out.write(&c,sizeof(c));
        out.write((char*)&author.authorName,NameSize);
        out.write(&c,sizeof(c));
        out.write((char*)&author.address,AddressSize);
        out.write(&c,sizeof(c));
        out.close();
        insertInPrimary(author.authorID,unique+2, PIFile);
    }
    else{
        short nextOf,deletedSize;
        out.seekg(header+1,ios::beg);
        out.read((char*)&nextOf,sizeof(nextOf));
        out.seekg(header-2,ios::beg);
        out.read((char*)&deletedSize,sizeof(deletedSize));
        short DifferSize= deletedSize-RecordSize;
        if(DifferSize<0){
            out.seekp(0,ios::end);
            unique=out.tellp();
            out.write((char*)&RecordSize,sizeof(RecordSize));
            out.write((char*)&author.authorID,IDSize);
            //delimiter
            char c='|';
            out.write(&c,sizeof(c));
            out.write((char*)&author.authorName,NameSize);
            out.write(&c,sizeof(c));
            out.write((char*)&author.address,AddressSize);
            out.write(&c,sizeof(c));
            out.close();
            insertInPrimary(author.authorID,unique+2, PIFile);
        }
        else if(DifferSize==0){
            unique=header;
            out.seekp(header-2,ios::beg);
            out.write((char*)&RecordSize,sizeof(RecordSize));
            out.write((char*)&author.authorID,IDSize);
            //delimiter
            char c='|';
            out.write(&c,sizeof(c));
            out.write((char*)&author.authorName,NameSize);
            out.write(&c,sizeof(c));
            out.write((char*)&author.address,AddressSize);
            out.write(&c,sizeof(c));
            out.seekp(0,ios::beg);
            out.write((char*)&nextOf,sizeof(nextOf));
            out.close();
            insertInPrimary(author.authorID,unique, PIFile);
        }
        else{

            if(DifferSize<6){
                out.seekp(0,ios::end);
                unique=out.tellp();
                out.write((char*)&RecordSize,sizeof(RecordSize));
                out.write((char*)&author.authorID,IDSize);
                //delimiter
                char c='|';
                out.write(&c,sizeof(c));
                out.write((char*)&author.authorName,NameSize);
                out.write(&c,sizeof(c));
                out.write((char*)&author.address,AddressSize);
                out.write(&c,sizeof(c));
                out.close();
                insertInPrimary(author.authorID,unique+2, PIFile);
            }
            else{
                out.seekp(0,ios::beg);
                unique=header;
                out.write((char*)&nextOf,sizeof(nextOf));
                out.seekp(header-2,ios::beg);
                out.write((char*)&RecordSize,sizeof(RecordSize));
                out.write((char*)&author.authorID,IDSize);
                //delimiter
                char c='|';
                out.write(&c,sizeof(c));
                out.write((char*)&author.authorName,NameSize);
                out.write(&c,sizeof(c));
                out.write((char*)&author.address,AddressSize);
                out.write(&c,sizeof(c));
                //seek to the begin of the record
                //delete the rest of the record
                out.seekp(header+RecordSize,ios::beg);
                DifferSize-=2;
                out.write((char*)&DifferSize,sizeof(DifferSize));
                char c2[]="9";
                out.write(c2,sizeof(c2));
                out.write(&c,sizeof(c));
                out.close();
                deleteRecord("9",OriginalFile, PIFile, SIFile, LLFile, type);
                insertInPrimary(author.authorID,unique, PIFile);
            }
        }
    }
}
void printAuthor();
void printBook();
