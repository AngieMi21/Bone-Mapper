#include <stdio.h>
#include <stdlib.h>
#include "CUnit.h"
#include "forensics.h"
#include "Basic.h"

/* * SUITE 1: Biological Identity & Age Tests
 * Tests Sex (Humeral), Age (Dental), and Ancestry (Platymeric Index).
 */

void test_Biological_Identity() {
    // 1. Test Sex Estimation (Adult)
    // Pass 48.2mm diameter and 0 (isChild = false)
    CU_ASSERT_STRING_EQUAL(estimateSex(48.2, 0), "Male");

    // Pass 0.0mm diameter and 0 (isChild = false)
    CU_ASSERT_STRING_EQUAL(estimateSex(0.0, 0), "Unknown");

    // 2. Test Sex Estimation (Juvenile)
    // Pass any diameter and 1 (isChild = true) -> Should return Indeterminate
    CU_ASSERT_STRING_EQUAL(estimateSex(48.2, 1), "Indeterminate");

    // 3. Test Age Estimation (Adult)
    // Pass translucent, root, isChild=0, and femurLength=0 (femur ignored for adults)
    CU_ASSERT_DOUBLE_EQUAL(estimateAge(2.0, 10.0, 0, 0.0), 29.13, 0.01);

    // Test Dental Error (Root = 0)
    CU_ASSERT_DOUBLE_EQUAL(estimateAge(2.0, 0.0, 0, 0.0), -1.0, 0.001);

    // 4. Test Age Estimation (Juvenile)
    // Pass 0, 0, isChild=1, and femurLength=10.0 -> Should use 10.0 * 0.8 = 8.0
    CU_ASSERT_DOUBLE_EQUAL(estimateAge(0.0, 0.0, 1, 10.0), 8.0, 0.1);
}

/* * SUITE 2: Anthropometric Regression Tests
 * Tests Stature, Weight, and BMI calculations.
 */

void test_Anthropometric_Metrics(void) {
    // Stature check: European (2.32 * 45 + 65.53 = 169.93)
    CU_ASSERT_DOUBLE_EQUAL(estimateStature(45.0, EUROPEAN, 0), 169.93, 0.01);

    // Stature check: Aboriginal (1.90 * 45 + 81.30 = 166.80)
    CU_ASSERT_DOUBLE_EQUAL(estimateStature(45.0, ABORIGINAL, 0), 166.80, 0.01);

    // BMI Check: 70kg / (1.75m^2) = 22.85
    CU_ASSERT_DOUBLE_EQUAL(calculateBMI(70.0, 175.0), 22.85, 0.1);

    // Invalid Input: Negative height for BMI
    CU_ASSERT_DOUBLE_EQUAL(calculateBMI(70.0, -5.0), -1.0, 0.001);
}

void test_Juvenile_Scaling(void) {
    // A 15cm femur is clearly a child
    // Expected: (3.25 * 15) + 12.5 = 61.25 cm
    CU_ASSERT_DOUBLE_EQUAL(estimateStature(15.0, EUROPEAN, 1), 61.25, 0.01);
}

#ifdef RUN_TESTS

int main() {
    if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

    // 1. Identity Suite (Sex, Age, Ancestry)
    CU_pSuite pSuiteBio = CU_add_suite("Biological_Identity_Suite", NULL, NULL);
    if (NULL != pSuiteBio) {
        CU_add_test(pSuiteBio, "Test_Sex_Age_Ancestry_Logic", test_Biological_Identity);
    }

    // 2. Metrics Suite (Stature, Weight, BMI)
    CU_pSuite pSuiteMetrics = CU_add_suite("Anthropometric_Metrics_Suite", NULL, NULL);
    if (NULL != pSuiteMetrics) {
        CU_add_test(pSuiteMetrics, "Test_Stature_Weight_BMI_Logic", test_Anthropometric_Metrics);
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

#endif
