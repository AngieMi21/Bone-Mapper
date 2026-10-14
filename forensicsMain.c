#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "forensics.h"

// Helper to clear the input buffer
void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

#ifndef RUN_TESTS

int main()
{
    ForensicCase *victim = (ForensicCase *)malloc(sizeof(ForensicCase));
    if (victim == NULL)
    {
        printf("Memory allocation failed!\n");
        return 1;
    }

    printf("|------ Forensic Case Management Engine ------|\n");

    // Validated Case ID
    while (1)
    {
        printf("Enter Case ID (Integer): ");
        fflush(stdout);
        if (scanf("%d", &victim->caseID) == 1 && victim->caseID > 0) break;
        printf("Invalid ID. Please enter a positive whole number.\n");
        while (getchar() != '\n');
    }

    // Range-Validated Measurements
    // Humeral Diameter (Typical range 30-60mm)
    while (1) {
        printf("Humeral Diameter (30.0-65.0 mm): ");
        fflush(stdout);
        if (scanf("%lf", &victim->humeralHeadDiam) == 1 &&
        		victim->humeralHeadDiam >= 30 &&
				victim->humeralHeadDiam <= 65)
        	break;
        printf("Error: Value out of biological range (30-65).\n");
        while (getchar() != '\n');
    }

    // Femur Length (Typical range 20-60cm, but accepts smaller for Juveniles)
    while (1) {
		printf("Enter Femur Length (cm): ");
		fflush(stdout);

		if (scanf("%lf", &victim->femurLength) == 1) {

			// 1. Juvenile Detection (1.0 to 14.9 cm)
			if (victim->femurLength >= 1.0 && victim->femurLength < 15.0) {
				victim->isChild = 1;
				printf("\n>>> JUVENILE REMAINS DETECTED: Switching to growth-chart scaling. <<<\n");
				break; // Exit loop for child
			}

			// 2. Adult Detection (15.0 to 60.0 cm)
			if (victim->femurLength >= 15.0 && victim->femurLength <= 60.0) {
				victim->isChild = 0;
				break; // Exit loop for adult
			}

			// 3. Error Case (Below 1cm or above 60cm)
			printf("Error: Value (%.1f) is outside biological norms (1-60cm).\n", victim->femurLength);
		} else {
			printf("Invalid input. Please enter a numeric value.\n");
		}

		while (getchar() != '\n'); // Clean the buffer
	}

    // Femur A-P & M-L (Typical range 15-50mm)
    while (1)
    {
		double minAP = (victim->isChild) ? 5.0 : 15.0;
		printf("Femur A-P Diameter (%.1f-50.0 mm): ", minAP); fflush(stdout);
		if (scanf("%lf", &victim->femurAP) == 1 && victim->femurAP >= minAP && victim->femurAP <= 50.0) break;
		printf("Error: Value out of range for %s remains.\n", victim->isChild ? "Juvenile" : "Adult");
		while (getchar() != '\n');
	}

	// 3. M-L Diameter Loop (Context-Aware)
	while (1) {
		double minML = (victim->isChild) ? 5.0 : 15.0;
		printf("Femur M-L Diameter (%.1f-50.0 mm): ", minML); fflush(stdout);
		if (scanf("%lf", &victim->femurML) == 1 && victim->femurML >= minML && victim->femurML <= 50.0) break;
		printf("Error: Value out of range for %s remains.\n", victim->isChild ? "Juvenile" : "Adult");
		while (getchar() != '\n');
	}

    // Dental Translucency & Root (Typical range 0-25mm)
    while (1)
    {
        printf("Dental Translucency Height (0.0-20.0 mm): ");
        fflush(stdout);
        if (scanf("%lf", &victim->translucentHeight) == 1 &&
        		victim->translucentHeight >= 0 &&
				victim->translucentHeight <= 20)
        	break;
        printf("Error: Value out of range (0-20).\n");
        while (getchar() != '\n');
    }

    while (1)
    {
        printf("Dental Root Height (5.0-30.0 mm): ");
        fflush(stdout);
        if (scanf("%lf", &victim->rootHeight) == 1 &&
        		victim->rootHeight >= 5 &&
				victim->rootHeight <= 30)
        	break;
        printf("Error: Value out of range (5-30).\n");
        while (getchar() != '\n');
    }

    // 3. Ancestry Selection
    printf("\nAncestry Selection:\n[1] Auto-Estimate\n[2] Manual\nChoice: ");
    fflush(stdout);

    int ancMode;

    scanf("%d", &ancMode);

    if (ancMode == 1)
    {
        victim->ancestry = autoEstimateAncestry(victim->femurAP, victim->femurML);
    } else
    {
        printf("Select (0:EU, 1:AF, 2:AS, 3:NA, 4:LA, 5:SA, 6:PI, 7:AB): ");
        fflush(stdout);
        int choice;
        scanf("%d", &choice);
        victim->ancestry = (Ancestry)choice;
    }

    // Multi-line Injury Logging
    while (getchar() != '\n'); // Clear buffer

    printf("\nInjury - Bone ");
    getMultiLineInjury(victim->injuries.bone, 100);

    printf("Injury - Tissue ");
    getMultiLineInjury(victim->injuries.tissue, 100);

    printf("Injury - Superficial ");
    getMultiLineInjury(victim->injuries.superficial, 100);

    // 5. Dynamic Filename and Saving
    char filePath[128];

    // Prepending "Reports/" tells the OS to save inside that specific folder.
    // Note: You must manually create the 'Reports' folder in your project directory first.
    sprintf(filePath, "Reports/Case_%d_Report.txt", victim->caseID);

    saveForensicReport(filePath, victim);

    printf("\n--- Analysis Complete ---\n");
    printf("Biological Profile generated and saved to '%s'.\n", filePath);
    fflush(stdout);

    free(victim);
    return 0;
}

#endif
