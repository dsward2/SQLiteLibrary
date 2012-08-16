/*
 * Copyright 2012 Dmitri Fedortchenko
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#import <Foundation/Foundation.h>
#import <sqlite3.h>

#define sqlite_now_epoch @"strftime('%s','now')"
#define make_nil_if_null(__string__) (__string__==nil||[__string__ isEqualToString:@"(null)"])?nil:__string__

#define sqlite3_column_nsstring(_statement_, __column__) \
							(char *)sqlite3_column_text(_statement_, __column__)? \
									make_nil_if_null([NSString stringWithUTF8String:(char *)sqlite3_column_text(_statement_, __column__)]):nil

#define sqlite3_to_string sqlite3_column_nsstring

typedef void (^SQLiteBlock)(sqlite3_stmt *compiledStatement);

@interface SQLiteLibrary : NSObject
{
	sqlite3 *database;
	NSRecursiveLock *lock;
}
+ (SQLiteLibrary *)singleton;

+ (void)setDatabaseFile:(NSString *)dbFilePath;

+ (void)setDatabaseFileInDocuments:(NSString *)dbFilename;

+ (void)setDatabaseFileInCache:(NSString *)dbFilename;


/**
* Begin transaction (singleton edition)
*/
+ (BOOL)begin;

+ (NSDictionary *)dictionaryForRowData:(sqlite3_stmt *)statement;

+ (NSArray*)performQueryAndGetResultList:(NSString*)query;


/**
* Commit transaction (singleton edition)
*/
+ (BOOL)commit;

/**
* See +performQuery:block:
*/
- (int64_t)performQueryInTransaction:(NSString *)query block:(SQLiteBlock)block;

/** Perform an SQL query. Works with any SQL query. (singleton edition)
* If no transaction has been started, the method will start a new transaction and auto-commit at the end of the query.
*
* @param query SQL query
* @param block Block with SQL result
* @return Returns different values depending on query: INSERT returns the id of the inserted row, UPDATE returns the number of affected rows, SELECT returns number of found rows
* */
+ (int64_t)performQuery:(NSString *)query block:(SQLiteBlock)block;

/**
* Copy database skeleton to user's documents directory.
* @param forceReset if True, overwrite existing database in user's documents directory
*/
+ (void)setupDatabaseAndForceReset:(BOOL)forceReset;

/**
* Begin transaction
*/
- (BOOL)begin;


/**
* Commit transaction
*/
- (BOOL)commit;


@end
