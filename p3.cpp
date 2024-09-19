#include <iostream>
#include <string>

#include "p3.h"

using namespace std;

bool compareContacts(const Contact *contact1, const Contact *contact2);

Info::Info() {
    this->name = "No Name Set";
    this->value = "No Value Set";
    this->next = nullptr;
}

Info::Info(std::string name, std::string value, Info *next) {
    this->name = name;
    this->value = value;
    this->next = next;
}

Contact::Contact() {
    this->first = "No First Set";
    this->last = "No Last Set";
    this->next = nullptr;
    this->headInfoList = nullptr;
}

Contact::Contact(std::string first, std::string last, Contact *next) {
    this->first = first;
    this->last = last;
    this->next = next;
    this->headInfoList = nullptr;
}

ContactList::ContactList() {
    this->headContactList = nullptr;
    this->count = 0;
}

int ContactList::getCount() {
    return this->count;
}

// print the specified contact and its information
// 1. return false and print nothing if the contact is not in the list
// 2. otherwise return true and print the contact
bool ContactList::printContact(std::ostream &os, std::string first, std::string last) {
    Contact *current = headContactList;

    while (current != nullptr) {
        if (current->first == first && current->last == last) {
            os << "Contact Name: " << current->first << " " << current->last << endl;
            Info *infoPtr = current->headInfoList;

            while (infoPtr != nullptr) {
                os << "\t" << infoPtr->name << " | " << infoPtr->value << endl;
                infoPtr = infoPtr->next;
            }

            return true;
        }

        current = current->next;
    }

    return false;
}

// print all contacts and their information
// print nothing if the list is empty
void ContactList::print(std::ostream &os) {
    Contact *current = headContactList;

    while (current != nullptr) {
        os << "Contact Name: " << current->first << " " << current->last << endl;
        Info *infoPtr = current->headInfoList;

        while (infoPtr != nullptr) {
            os << "\t" << infoPtr->name << " | " << infoPtr->value << endl;
            infoPtr = infoPtr->next;
        }

        current = current->next;
    }
}

// add a contact to the back of the list
// 1. return false and do nothing if the contact is already in the list
// 2. otherwise return true and add the contact to the back of the list
// - do not forget to update count
bool ContactList::addContact(std::string first, std::string last) {
    Contact *current = headContactList;
    while (current != nullptr) {
        if (current->first == first && current->last == last) {
            // cout << "Test 1" << endl;
            return false;
        }

        current = current->next;
    }
    
    Contact *newContact = new Contact(first, last);
    if (headContactList == nullptr) {
        headContactList = newContact;
    }
    
    else {
        Contact *tail = headContactList;

        while (tail->next != nullptr) {
            tail = tail->next;
        }

        tail->next = newContact;
    }
    
    count++;
    
    return true;
}

// add info to the back of a contact's info list
// 1. return false and do nothing if the contact is not in the list
// 2. if the infoName is already in the info list, update the infoValue and return true
// 3. otherwise add the info to the back of the contact's list and return true
bool ContactList::addInfo(std::string first, std::string last, std::string infoName, std::string infoVal) {
    Contact *current = headContactList;

    while (current != nullptr) {
        if (current->first == first && current->last == last) {
            Info *infoPtr = current->headInfoList;

            while (infoPtr != nullptr) {
                if (infoPtr->name == infoName) {
                    infoPtr->value = infoVal;
                    return true;
                }

                infoPtr = infoPtr->next;
            }

            Info *newInfo = new Info(infoName, infoVal);

            if (current->headInfoList == nullptr) {
                current->headInfoList = newInfo;
            }

            else {
                Info *lastInfo = current->headInfoList;

                while (lastInfo->next != nullptr) {
                    lastInfo = lastInfo->next;
                }

                lastInfo->next = newInfo;
            }

            return true;
        }

        current = current->next;
    }

    return false;
}

// add a contact to the list in ascending order by last name
//     if last names are equal, then order by first name ascending
// 1. return false and do nothing if the contact is already in the list
// 2. otherwise return true and add the contact to the list
// - do not forget to update count
// - compare strings with the built-in comparison operators: <, >, ==, etc.
// - a compare method/function is recommended

bool compareContacts(const Contact *contact1, const Contact *contact2) {
    if (contact1->last < contact2->last) {
        return true;
    }
    
    else if (contact1->last == contact2->last) {
        return contact1->first < contact2->first;
    }

    return false;
}

bool ContactList::addContactOrdered(std::string first, std::string last) {
    Contact *current = headContactList;

    while (current != nullptr) {
        if (current->first == first && current->last == last) {
            // cout << "Test 2" << endl;
            return false;
        }

        current = current->next;
    }

    Contact *newContact = new Contact(first, last);

    if (headContactList == nullptr || compareContacts(newContact, headContactList)) {
        newContact->next = headContactList;
        headContactList = newContact;
    }
    
    else {
        Contact *prev = headContactList;
        current = headContactList->next;

        while (current != nullptr && compareContacts(current, newContact)) {
            prev = current;
            current = current->next;
        }

        prev->next = newContact;
        newContact->next = current;
    }

    count++;
    return true;
}

// add info to a contact's info list in ascending order by infoName
// 1. return false and do nothing if the contact is not in the list
// 2. if the infoName is already in the info list, update the infoValue and return true
// 3. otherwise add the info to the contact's list and return true
bool ContactList::addInfoOrdered(std::string first, std::string last, std::string infoName, std::string infoVal) {
    Contact *current = headContactList;

    while (current != nullptr) {
        if (current->first == first && current->last == last) {
            Info *newInfo = new Info(infoName, infoVal);
            Info *prev = nullptr;
            Info *currentInfo = current->headInfoList;

            while (currentInfo != nullptr && currentInfo->name < infoName) {
                prev = currentInfo;
                currentInfo = currentInfo->next;
            }

            if (currentInfo != nullptr && currentInfo->name == infoName) {
                currentInfo->value = infoVal;
                delete newInfo;
                return true;
            }

            if (prev == nullptr) {
                newInfo->next = current->headInfoList;
                current->headInfoList = newInfo;
            }

            else {
                newInfo->next = currentInfo;
                prev->next = newInfo;
            }

            return true;
        }

        current = current->next;
    }

    return false;
}

// remove the contact and its info from the list
// 1. return false and do nothing if the contact is not in the list
// 2. otherwise return true and remove the contact and its info
// - do not forget to update count
bool ContactList::removeContact(std::string first, std::string last) {
    if (headContactList == nullptr)
        return false;

    if (headContactList->first == first && headContactList->last == last) {
        Contact *temp = headContactList;

        headContactList = headContactList->next;
        delete temp;
        count--;
        return true;
    }

    Contact *prev = headContactList;
    Contact *current = headContactList->next;

    while (current != nullptr) {
        if (current->first == first && current->last == last) {
            prev->next = current->next;
            delete current;
            count--;
            return true;
        }

        prev = current;
        current = current->next;
    }

    return false;
}

// remove the info from a contact's info list
// 1. return false and do nothing if the contact is not in the list
// 2. return false and do nothing if the info is not in the contact's info list
// 3. otherwise return true and remove the info from the contact's list
bool ContactList::removeInfo(std::string first, std::string last, std::string infoName) {
    Contact *current = headContactList;

    while (current != nullptr) {
        if (current->first == first && current->last == last) {
            Info *prev = nullptr;
            Info *currentInfo = current->headInfoList;

            while (currentInfo != nullptr) {
                if (currentInfo->name == infoName) {
                    if (prev == nullptr) {
                        current->headInfoList = currentInfo->next;
                    }
                    
                    else {
                        prev->next = currentInfo->next;
                    }

                    delete currentInfo;
                    return true;
                }

                prev = currentInfo;
                currentInfo = currentInfo->next;
            }

            return false;
        }

        current = current->next;
    }

    return false;
}

// destroy the list by removing all contacts and their infos
ContactList::~ContactList() {
    Contact *current = headContactList;

    while (current != nullptr) {
        Contact *temp = current;
        current = current->next;

        while (temp->headInfoList != nullptr) {
            Info *infoTemp = temp->headInfoList;
            temp->headInfoList = temp->headInfoList->next;
            delete infoTemp;
        }

        delete temp;
    }
}

// deep copy the source list
// - do not forget to update count
ContactList::ContactList(const ContactList &src) {
    headContactList = nullptr;
    count = 0;
    Contact *srcCurrent = src.headContactList;

    while (srcCurrent != nullptr) {
        addContact(srcCurrent->first, srcCurrent->last);
        Info *srcInfoCurrent = srcCurrent->headInfoList;
        Contact *destCurrent = headContactList;

        while (destCurrent != nullptr && (destCurrent->first != srcCurrent->first || destCurrent->last != srcCurrent->last)) {
            destCurrent = destCurrent->next;
        }

        while (srcInfoCurrent != nullptr) {
            addInfo(destCurrent->first, destCurrent->last, srcInfoCurrent->name, srcInfoCurrent->value);
            srcInfoCurrent = srcInfoCurrent->next;
        }

        srcCurrent = srcCurrent->next;
    }
}

// remove all contacts and their info then deep copy the source list
// - do not forget to update count
const ContactList &ContactList::operator=(const ContactList &src) {
    if (this != &src) {
        this->~ContactList();
        headContactList = nullptr;
        count = 0;
        Contact *srcCurrent = src.headContactList;

        while (srcCurrent != nullptr) {
            addContact(srcCurrent->first, srcCurrent->last);
            Info *srcInfoCurrent = srcCurrent->headInfoList;
            Contact *destCurrent = headContactList;

            while (destCurrent != nullptr && (destCurrent->first != srcCurrent->first || destCurrent->last != srcCurrent->last)) {
                destCurrent = destCurrent->next;
            }

            while (srcInfoCurrent != nullptr) {
                addInfo(destCurrent->first, destCurrent->last, srcInfoCurrent->name, srcInfoCurrent->value);
                srcInfoCurrent = srcInfoCurrent->next;
            }

            srcCurrent = srcCurrent->next;
        }
    }
    return *this;
}
