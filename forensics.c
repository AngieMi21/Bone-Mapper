#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "forensics.h"

void getMultiLineInjury(char* buffer, int maxLen) {
    char line[100];
    buffer[0] = '\0';
    printf("(Press Enter twice to finish this section)\n");
    fflush(stdout);

    while (fgets(line, sizeof(line), stdin)) {
        // Robust check for the "Double Enter" exit
        if (line[0] == '\n' || line[0] == '\r') {
            break;
        }

        // Check buffer safety (HRM Efficiency: O(1) length check)
        if (strlen(buffer) + strlen(line) < (size_t)maxLen - 1) {
            strcat(buffer, line);
        } else {
            break;
        }
    }
}

// Logic: Based on standard forensic thresholds (approx. 45.0mm)
const char* estimateSex(double diameter, int isChild)
{
	if (isChild) return "Indeterminate";

    // Boundary Testing: We treat 0 or negative as "Invalid/Erroneous" Data
    if (diameter <= 0)
    	return "Unknown";

    // Discriminant Function: Diameter - 45.5 (Threshold)
    return (diameter > 45.5) ? "Male" : "Female";
}

double estimateAge(double translucent, double root, int isChild, double femurLength)
{
	if (isChild) return (femurLength * 0.8);

    if (translucent < 0 || root <= 0) return -1.0;
    // Lamendin formula: (0.18 * (Trans/Root)*100) + 25.53
    return (0.18 * (translucent / root) * 100.0) + 25.53;
}

Ancestry autoEstimateAncestry(double ap, double ml)
{
    if (ap <= 0 || ml <= 0)
    	return EUROPEAN; // Safety Fallback
    double pi = (ap / ml) * 100.0;
    // 1. PLATYMERIC (< 84.9): Flattened shaft
    // Primarily Asian and Indigenous American populations
    if (pi < 85.0)
    {
        return NATIVE_AMERICAN;
    }
    // 2. STENOMERIC (> 100.0): Round/Pillared shaft
    // Primarily African and Australian Aboriginal populations
    else if (pi > 100.0)
    {
        return AFRICAN;
    }
    // 3. EURYMERIC (85.0 - 99.9): Average/Oval shaft
    // Primarily European, Latino, and South Asian populations
    else
    {
        return EUROPEAN;
    }
}

// Logic: Regression formulas (Trotter & Gleser, 1952 simplified)
double estimateStature(double boneLength, Ancestry origin, int isChild)
{
	if (boneLength <= 0) return -1.0;

    // JUVENILE LOGIC: Children have higher limb-to-height ratios
	if (isChild)
	{
		// Simplified growth-plate regression for juvenile remains
		return (3.25 * boneLength) + 12.5;
	}

    switch(origin) {
        case AFRICAN:          return (2.10 * boneLength) + 72.22;
        case ASIAN:            return (2.15 * boneLength) + 72.50;
        case NATIVE_AMERICAN:  return (2.26 * boneLength) + 66.38;
        case LATINO:           return (2.16 * boneLength) + 75.50;
        case SOUTH_ASIAN:      return (2.23 * boneLength) + 69.50;
        case PACIFIC_ISLANDER: return (2.14 * boneLength) + 73.10;
        case ABORIGINAL:       return (1.90 * boneLength) + 81.30;
        case EUROPEAN:
        default:               return (2.32 * boneLength) + 65.53;
    }
}

double calculateWeight(double statureCm, int isChild)
{
	if (statureCm <= 0) return 0.0;
	    if (isChild) return (statureCm * 0.15);
	    double wt = (statureCm * 0.9) - 95.0;
	    return (wt > 0) ? wt : 2.0;
}

double calculateBMI(double weightKg, double statureCm)
{
    if (statureCm <= 0 || weightKg <= 0) return -1.0;
    double m = statureCm / 100.0;
    return weightKg / (m * m);
}

void saveForensicReport(const char* filename, ForensicCase* victim)
{
    FILE *f = fopen(filename, "w");
    if (!f) return;

    // 1. Resolve labels based on case data
    const char* bioClass = (victim->isChild) ? "JUVENILE" : "ADULT";
    const char* ageFormula = (victim->isChild) ? "Maresh Growth Scaling" : "Lamendin Dental Method";
    const char* statureMethod = (victim->isChild) ? "Juvenile Scaling" : "Trotter-Gleser Regression";

    // 2. Ancestry Logic
    const char* ancestryMethod;
    const char* ancestryName;

    if (victim->femurAP > 0 && victim->femurML > 0) {
        victim->ancestry = autoEstimateAncestry(victim->femurAP, victim->femurML);
        ancestryMethod = "Automated (Platymeric Index)";
    } else {
        // Default to European/Global Mean if measurements are missing
        victim->ancestry = EUROPEAN;
        ancestryMethod = "Default (Global Mean)";
    }

    // Map enum to string for the report
    switch(victim->ancestry) {
        case AFRICAN:         ancestryName = "African"; break;
        case ASIAN:           ancestryName = "Asian"; break;
        case NATIVE_AMERICAN: ancestryName = "Native American"; break;
        default:              ancestryName = "European"; break;
    }

    // 3. Perform Final Forensic Calculations
    // Contextual logic ensures we don't apply adult formulas to juvenile remains.

    // STATURE: Pass ancestry and child flag
    double stCm = estimateStature(victim->femurLength, victim->ancestry, victim->isChild);
    // WEIGHT: Contextual check prevents negative results
    double wtKg = calculateWeight(stCm, victim->isChild);
    // AGE: If child, use growth proxy; if adult, use dental formula
    double age  = estimateAge(victim->translucentHeight, victim->rootHeight, victim->isChild, victim->femurLength);
    // SEX: Report as indeterminate for pre-pubescent remains
    const char* sexLabel = estimateSex(victim->humeralHeadDiam, victim->isChild);
    // BMI: Final safety check for division by zero or negative weight
    double bmi  = calculateBMI(wtKg, stCm);

    // 4. Print Block formatted for the "The Bone Mapper" Engine
    fprintf(f, "|-------------- BIOLOGICAL PROFILE --------------|\n");
    fprintf(f, "=================================================\n");
    fprintf(f, "CASE ID: %-10d | BIOLOGICAL CLASS: %s\n", victim->caseID, bioClass);
    fprintf(f, "EST. SEX: %-9s | EST. AGE: %.1f years\n", sexLabel, age);
    fprintf(f, "AGE METHOD: %s\n", ageFormula);
    fprintf(f, "ANCESTRY: %-9s | METHOD: %s\n", ancestryName, ancestryMethod);
    fprintf(f, "------------------------------------------------\n");
    fprintf(f, "STATURE:  %.1f cm | METHOD: %s\n", stCm, statureMethod);
    fprintf(f, "WEIGHT:   %.1f kg  ", wtKg);
    if (victim->isChild) {
            // Scientifically, we skip BMI for juveniles
            fprintf(f, "| BMI:   N/A (Juvenile Class)\n");
        } else {
            double bmi = calculateBMI(wtKg, stCm);
            fprintf(f, "| BMI:      %.1f\n", bmi);
        }
    fprintf(f, "------------------------------------------------\n");
    fprintf(f, "INJURY LOG:\n [BONE]\n%s\n [TISSUE]\n%s\n [SUPERFICIAL]\n%s\n",
            victim->injuries.bone, victim->injuries.tissue, victim->injuries.superficial);
    fprintf(f, "================================================\n");

    fclose(f);
}
