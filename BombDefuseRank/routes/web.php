<?php

use Illuminate\Support\Facades\Route;
use App\Http\Controllers\DashboardController; 
use App\Http\Controllers\HomeController; 
use App\Http\Controllers\QueueController; 
use App\Http\Controllers\ArduinoController; 
use Illuminate\Support\Facades\Auth;

#isso nao pode pedir login
#é a comunicacao da bomba com este sistema
Route::post('/arduino-data/{team}', [ArduinoController::class, 'receiveArduinoDataFromPython']);

Route::group([], function () {
    Route::get('/', [HomeController::class, 'index'])->name('home');
    Route::get('/rank', [HomeController::class, 'rank'])->name('rank');
    Route::get('/fila', [HomeController::class, 'fila'])->name('fila');
});

Route::middleware(['auth'])->group(function () {
    Route::get('/dashboard', [DashboardController::class, 'index'])->name('dashboard');
    Route::get('/new', [DashboardController::class, 'new'])->name('new');
    Route::post('/save', [DashboardController::class, 'save'])->name('save');
    Route::get('/foto/{team}', [DashboardController::class, 'foto'])->name('foto');
    Route::post('/update/{team}', [DashboardController::class, 'update'])->name('update');
    Route::get('/edit/{team}', [DashboardController::class, 'edit'])->name('edit');

    // Fila
    Route::delete('/fila/{id}',[QueueController::class, 'destroy'])->name('fila.destroy');
    Route::put('/fila/{id}', [QueueController::class, 'update'])->name('fila.update');
    Route::get('/fila/{id}/editar', [QueueController::class, 'edit'])->name('fila.edit');
    Route::get('/fila/cadastrar', [QueueController::class, 'create'])->name('fila.create');
    Route::post('/fila', [QueueController::class, 'store'])->name('fila.store');
});

Route::middleware(['auth', 'permission:admin'])->group(function () {
    Route::get('/delete/{team}', [DashboardController::class, 'delete'])->name('delete');
});

require __DIR__.'/auth.php';
