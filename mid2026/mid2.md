# SQLite를 활용한 도서관 회원 및 대출 관리 DB 설계 및 구현

## 1. 프로젝트 시나리오

본 프로젝트는 **'대학 도서관 회원 및 대출 관리 시스템'**을 시나리오로 설정하여 설계되었습니다.  
도서관 회원의 인적사항을 관리하는 `members` 테이블과 회원의 도서 대출 정보를 관리하는 `loans` 테이블을 구축하고, 회원 검색 및 대출 현황 조회 기능을 구현합니다.

---

## 2. 데이터베이스 스키마 설계

### 2.1 테이블 구조

#### 1) `members` (회원 테이블)
| 컬럼명 (Column) | 데이터 타입 (Type) | 제약 조건 (Constraints) | 설명 |
| :--- | :--- | :--- | :--- |
| `member_id` | INTEGER | PRIMARY KEY AUTOINCREMENT | 회원 고유 번호 |
| `name` | TEXT | NOT NULL | 회원 이름 |
| `email` | TEXT | UNIQUE NOT NULL | 이메일 주소 |
| `phone` | TEXT | NOT NULL | 연락처 |
| `join_date` | TEXT | DEFAULT CURRENT_DATE | 가입일자 |

#### 2) `loans` (대출 테이블)
| 컬럼명 (Column) | 데이터 타입 (Type) | 제약 조건 (Constraints) | 설명 |
| :--- | :--- | :--- | :--- |
| `loan_id` | INTEGER | PRIMARY KEY AUTOINCREMENT | 대출 고유 번호 |
| `member_id` | INTEGER | FOREIGN KEY (`members.member_id`) | 대출한 회원 번호 |
| `book_title` | TEXT | NOT NULL | 대출 도서명 |
| `loan_date` | TEXT | NOT NULL | 대출일자 |
| `status` | TEXT | CHECK (`status` IN ('대출중', '반납완료')) | 대출 상태 |

---

## 3. SQL 구현 스크립트

### 3.1 테이블 생성 (CREATE)

```sql
-- 기존 테이블 삭제 (초기화용)
DROP TABLE IF EXISTS loans;
DROP TABLE IF EXISTS members;

-- 회원 테이블 생성
CREATE TABLE members (
    member_id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    email TEXT UNIQUE NOT NULL,
    phone TEXT NOT NULL,
    join_date TEXT DEFAULT (DATE('now'))
);

-- 대출 테이블 생성
CREATE TABLE loans (
    loan_id INTEGER PRIMARY KEY AUTOINCREMENT,
    member_id INTEGER NOT NULL,
    book_title TEXT NOT NULL,
    loan_date TEXT NOT NULL,
    status TEXT CHECK (status IN ('대출중', '반납완료')) DEFAULT '대출중',
    FOREIGN KEY (member_id) REFERENCES members (member_id) ON DELETE CASCADE
);
```

### 3.2 데이터 삽입 (INSERT - 4명 이상의 회원 데이터)

```sql
-- 1. 회원 데이터 삽입 (총 4명)
INSERT INTO members (name, email, phone, join_date) VALUES 
('김철수', 'chulsoo.kim@example.com', '010-1234-5678', '2024-01-15'),
('이영희', 'younghee.lee@example.com', '010-2345-6789', '2024-02-01'),
('박민수', 'minsu.park@example.com', '010-3456-7890', '2024-03-10'),
('정수진', 'sujin.jung@example.com', '010-4567-8901', '2024-04-05');

-- 2. 대출 데이터 삽입
INSERT INTO loans (member_id, book_title, loan_date, status) VALUES 
(1, 'C 언어 프로그래밍 입문', '2024-10-01', '반납완료'),
(1, 'SQLite 데이터베이스 완벽 가이드', '2024-10-20', '대출중'),
(2, '파이썬 데이터 분석', '2024-10-18', '대출중'),
(3, '운영체제론', '2024-09-15', '반납완료'),
(4, '자료구조와 알고리즘', '2024-10-22', '대출중');
```

---

## 4. 데이터 검색 및 조회 (SELECT)

### 4.1 전체 회원 목록 조회
```sql
SELECT * FROM members;
```

### 4.2 특정 회원 검색 (이름 키워드 검색)
```sql
-- 이름에 '수'가 포함된 회원 검색
SELECT member_id, name, email, phone 
FROM members 
WHERE name LIKE '%수%';
```

### 4.3 회원별 대출 현황 조인(JOIN) 검색
```sql
-- 회원의 이름과 해당 회원이 대출한 도서명 및 대출 상태 함께 조회
SELECT 
    m.member_id,
    m.name AS 회원이름,
    l.book_title AS 대출도서,
    l.loan_date AS 대출일자,
    l.status AS 대출상태
FROM members m
INNER JOIN loans l ON m.member_id = l.member_id
ORDER BY m.member_id ASC;
```

### 4.4 현재 '대출중'인 회원 및 도서 검색
```sql
SELECT 
    m.name AS 회원이름,
    m.phone AS 연락처,
    l.book_title AS 미반납도서,
    l.loan_date AS 대출일자
FROM members m
JOIN loans l ON m.member_id = l.member_id
WHERE l.status = '대출중';
```

---

## 5. 실행 방법 (SQLite CLI 기준)

1. 터미널에서 SQLite 데이터베이스 실행:
   ```bash
   sqlite3 library.db
   ```
2. 출력을 보기 좋게 설정:
   ```sql
   .mode column
   .headers on
   ```
3. 위의 SQL 스크립트를 순서대로 실행하여 테이블 생성, 데이터 삽입 및 검색 결과를 확인합니다.
