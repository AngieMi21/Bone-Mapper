# Bone-Mapper
A C-based software application and testing suite designed to process anthropological data, estimate biological profiles, and manage forensic case reports. The project integrates standard anthropometric and forensic regression formulas to analyze human remains while incorporating a robust unit testing framework (CUnit) for internal verification.

Key FeaturesBiological Profile Estimation:Computes sex estimation based on humeral head diameters and evaluates age using dental metrics (such as the Lamendin method).   

Context-aware handling to distinguish between adult and juvenile remains, utilizing growth-scaling proxies (like Maresh growth scaling) for children.   

Anthropometric Metrics:Calculates stature based on bone lengths across various population ancestries using Trotter-Gleser regression formulas.   
Computes body weight and Body Mass Index (BMI), with safety bounds that omit inappropriate metrics (such as BMI) for juvenile remains.   

Automated Ancestry Indexing:Automatically estimates ancestry using the Platymeric Index derived from femoral anteroposterior (A-P) and mediolateral (M-L) diameters, categorizing findings into classifications like Platymeric, Stenomeric, or Eurymeric.   

Multi-line Injury Logging:Interactive terminal input allowing comprehensive multi-line logging across bone, tissue, and superficial trauma categories.   S

tructured Forensic Reporting:Automatically generates and exports formatted case summary files (e.g., inside a designated Reports/ directory) containing complete biological profiles, applied methodologies, and injury logs.   

Unit Testing Suite:Built-in test harnesses utilizing CUnit to thoroughly test biological identity logic, anthropometric regression calculations, and memory tracking routines.
