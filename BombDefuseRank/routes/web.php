<?php

use Illuminate\Support\Facades\Route;

/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| Here is where you can register web routes for your application. These
| routes are loaded by the RouteServiceProvider within a group which
| contains the "web" middleware group. Now create something great!
|
*/

Route::get('/', [App\Http\Controllers\WelcomeController::class, 'index'])->name('home');

#Auth::routes(['register' => false]);
Auth::routes();

Route::get('/home', [App\Http\Controllers\HomeController::class, 'index'])->name('dashboard');

Route::get('/new', [App\Http\Controllers\HomeController::class, 'new'])->name('new');
Route::post('/save', [App\Http\Controllers\HomeController::class, 'save'])->name('save');
Route::get('/foto/{team}', [App\Http\Controllers\HomeController::class, 'foto'])->name('foto');
Route::post('/update/{team}', [App\Http\Controllers\HomeController::class, 'update'])->name('update');
Route::get('/edit/{team}', [App\Http\Controllers\HomeController::class, 'edit'])->name('edit');
Route::get('/delete/{team}', [App\Http\Controllers\HomeController::class, 'delete'])->name('delete');

Route::post('/arduino-data/{team}', [App\Http\Controllers\HomeController::class, 'receiveArduinoDataFromPython']);