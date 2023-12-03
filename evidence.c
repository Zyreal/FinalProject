#include "defs.h"

// Initializing the list of evidence
void initEvidenceList(EvidenceListType* el) {
    el->head = NULL;
    el->tail = NULL;
    el->size = 0;
}

// Cleaning up the evidence
void cleanupEvidence(EvidenceListType* el) {
    EvidenceNodeType *head = el->head;
    EvidenceNodeType *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}