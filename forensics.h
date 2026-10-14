#ifndef FORENSICS_H
#define FORENSICS_H

#define MAX_INJURY_LEN 500

typedef enum {
    EUROPEAN, AFRICAN, ASIAN, NATIVE_AMERICAN,
    LATINO, SOUTH_ASIAN, PACIFIC_ISLANDER, ABORIGINAL
} Ancestry;

typedef struct {
    char bone[MAX_INJURY_LEN];
    char tissue[MAX_INJURY_LEN];
    char superficial[MAX_INJURY_LEN];
} InjuryLog;

typedef struct {
    int caseID;
    int isChild;               // 1 for Juvenile, 0 for Adult
    double humeralHeadDiam;    // Double for Sex estimation
    double femurLength;        // Double for Stature
    double femurAP;            // Double for Ancestry
    double femurML;
    double translucentHeight;  // Double for Age
    double rootHeight;
    Ancestry ancestry;
    InjuryLog injuries;
} ForensicCase;

// Function Prototypes
void getMultiLineInjury(char* buffer, int maxLen);
const char* estimateSex(double diameter, int isChild);
double estimateAge(double translucent, double root, int isChild, double femurLength);
Ancestry autoEstimateAncestry(double ap, double ml);
double estimateStature(double boneLength, Ancestry origin, int isChild);
double calculateBMI(double weightKg, double statureCm);
double calculateWeight(double statureCm, int isChild);
void saveForensicReport(const char* filename, ForensicCase* v);

#endif
