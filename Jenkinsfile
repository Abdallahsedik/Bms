pipeline {
    agent any

    options {
        buildDiscarder(logRotator(numToKeepStr: '10')) // Keep audit trail clean
        timestamps()
    }

    stages {
        stage('1. Clean & Checkout') {
            steps {
                checkout scm
                sh 'make clean'
            }
        }

        stage('2. Cross-Compile (S32K144)') {
            steps {
                echo 'Building S32K144 target...'
                sh 'make build_target'
            }
        }

        stage('3. Static Analysis (MISRA-C)') {
            steps {
                echo 'Running Cppcheck...'
                // Run cppcheck strictly with MISRA rules. Output to XML format.
                sh 'cppcheck --enable=all --addon=misra.json --xml --xml-version=2 src/ 2> cppcheck-result.xml'
            }
            post {
                always {
                    // Record issues. Marks build unstable on MISRA violations.
                    recordIssues tool: cppCheck(pattern: 'cppcheck-result.xml'), qualityGates: [[threshold: 1, type: 'TOTAL', unstable: true]]
                }
            }
        }

        stage('4. Native Unit Tests (Host)') {
            steps {
                echo 'Running native unit tests with hardware stubs...'
                sh 'make test_host'
            }
            // Note: If you add the JUnit plugin to Unity, you would parse the XML here:
            // post { always { junit 'build/test-results.xml' } }
        }

        stage('5. Archive Traceability Artifacts') {
            steps {
                echo 'Archiving .elf and .map files...'
                archiveArtifacts artifacts: 'build/*.elf, build/*.map', fingerprint: true
            }
        }
    }
}