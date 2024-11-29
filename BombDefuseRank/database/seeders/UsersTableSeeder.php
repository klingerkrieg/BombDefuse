<?php

namespace Database\Seeders;

use Illuminate\Database\Seeder;
use App\Models\User;
use Illuminate\Support\Facades\Hash;

class UsersTableSeeder extends Seeder
{
    /**
     * Run the database seeds.
     */
    public function run(): void
    {

        User::create([
            'username' => "@org",
            'password' => Hash::make('grandepanela2022'),
        ])->givePermissionTo('org');

        User::create([
            'username' => '@admin',
            'password' => Hash::make('grandepanela2022'),
        ])->givePermissionTo('admin');

    }
}
