pipeline {
    agent any

    options {
        buildDiscarder(logRotator(numToKeepStr: '10'))
        timestamps()
    }

    stages {
        stage('1. Clean & Checkout') {
            steps {
                checkout scm
                bat 'make clean'
            }
        }

        stage('2. Cross-Compile (S32K144)') {
            steps {
                echo 'Building S32K144 target...'
                bat 'make build_target'
            }
        }

        stage('3. Static Analysis (MISRA-C)') {
            steps {
                echo 'Running Cppcheck...'
                // Redirecting stdout (>) to the xml file
                bat 'cppcheck --enable=all --suppress=missingInclude --xml --xml-version=2 mcu/ 2> cppcheck-result.xml || exit 0'
            }
            post {
                always {
                    recordIssues tool: cppCheck(pattern: 'cppcheck-result.xml'), qualityGates: [[threshold: 1, type: 'TOTAL', unstable: true]]
                }
            }
        }

        stage('4. Native Unit Tests (Host)') {
            steps {
                echo 'Building and running unit tests...'
                bat 'make test_host'
            }
            
        }

        stage('5. Archive Traceability Artifacts') {
            steps {
                echo 'Archiving library...'
                archiveArtifacts artifacts: 'build/*.a', fingerprint: true
            }
        }
    }
}